/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ObjFile
*/

#include "ObjFile.hpp"
#include "ArgumentMap.hpp"
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace RayTracer {

    ObjFile::ObjFile(ArgumentMap params) : fileName(params["filename"].as<std::string>()), triangleLib("./Plugins/Primitives/libTriangle.so")
    {
        parseObjFile(fileName);
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>())
            material.setColor(params["color"].as<Color>());
        _position = params["position"].as<Math::Point3D>();
        auto res =  generateTriangles();
        tree = std::make_unique<Node>(std::move(res));
    }

    ObjFile::~ObjFile()
    {
    }

    std::vector<std::unique_ptr<IPrimitive>> ObjFile::generateTriangles()
    {
        std::vector<std::unique_ptr<IPrimitive>> triangles;
        auto constructor = triangleLib.getConstructor<IPrimitive, ArgumentMap>("createPrimitive");
        for (auto &indices : this->_sides) {
            auto vertex1 = vertices[indices.getVertices()[0] - 1];
            auto vertex2 = vertices[indices.getVertices()[1] - 1];
            auto vertex3 = vertices[indices.getVertices()[2] - 1];
            ArgumentMap params;

            ArgumentMap vertexParam;
            vertexParam["x"] = vertex1.getX() + _position.getX();
            vertexParam["y"] = vertex1.getY() + _position.getY();
            vertexParam["z"] = vertex1.getZ() + _position.getZ();
            params["vertex1"] = vertexParam;

            ArgumentMap vertexParam2;
            vertexParam2["x"] = vertex2.getX() + _position.getX();
            vertexParam2["y"] = vertex2.getY() + _position.getY();
            vertexParam2["z"] = vertex2.getZ() + _position.getZ();
            params["vertex2"] = vertexParam2;

            ArgumentMap vertexParam3;
            vertexParam3["x"] = vertex3.getX() + _position.getX();
            vertexParam3["y"] = vertex3.getY() + _position.getY();
            vertexParam3["z"] = vertex3.getZ() + _position.getZ();
            params["vertex3"] = vertexParam3;

            if (indices.getTextureCoords()[0] > 0 &&
                indices.getTextureCoords()[1] > 0 &&
                indices.getTextureCoords()[2] > 0 &&
                indices.getTextureCoords()[0] <= textureCoords.size() &&
                indices.getTextureCoords()[1] <= textureCoords.size() &&
                indices.getTextureCoords()[2] <= textureCoords.size()) {
                ArgumentMap textureParam1;
                textureParam1["x"] = textureCoords[indices.getTextureCoords()[0] - 1].getX();
                textureParam1["y"] = textureCoords[indices.getTextureCoords()[0] - 1].getY();
                textureParam1["z"] = textureCoords[indices.getTextureCoords()[0] - 1].getZ();
                params["textureV1"] = textureParam1;

                ArgumentMap textureParam2;
                textureParam2["x"] = textureCoords[indices.getTextureCoords()[1] - 1].getX();
                textureParam2["y"] = textureCoords[indices.getTextureCoords()[1] - 1].getY();
                textureParam2["z"] = textureCoords[indices.getTextureCoords()[1] - 1].getZ();
                params["textureV2"] = textureParam2;
                ArgumentMap textureParam3;
                textureParam3["x"] = textureCoords[indices.getTextureCoords()[2] - 1].getX();
                textureParam3["y"] = textureCoords[indices.getTextureCoords()[2] - 1].getY();
                textureParam3["z"] = textureCoords[indices.getTextureCoords()[2] - 1].getZ();
                params["textureV3"] = textureParam3;
            }
            const auto& matName = indices.getMaterialName();
            if (material.getMaterialList().find(matName) != material.getMaterialList().end()) {
                params["material"] = std::ref(material.getMaterialList()[matName]);
            } else {
                params["material"] = std::ref(material);
            }
            params["color"] = material.getColor();
            triangles.push_back(constructor(params));
        }
        return triangles;
    }

    void ObjFile::parseMltFile(std::string fileName)
    {
        std::fstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Unable to open material file: " << fileName << std::endl;
            return;
        }
        std::string currentMaterial = "default";
        std::string buffer;
        while (std::getline(file, buffer)) {
            buffer.erase(0, buffer.find_first_not_of(" \t"));
            if (buffer.rfind("newmtl ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(7));
                bufferStream >> currentMaterial;
                Material newMaterial;
                newMaterial.setName(currentMaterial);
                material.getMaterialList()[currentMaterial] = newMaterial;
            }
            if (buffer.rfind("Kd ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(3));
                double r, g, b;
                bufferStream >> r >> g >> b;
                material.setColor(Color(r, g, b));
            }
            if (buffer.rfind("d ", 0) == 0) {
                std::stringstream ss(buffer.substr(2));
                double opacity;
                ss >> opacity;
                material.getMaterialList()[currentMaterial].setOpacity(opacity);
            }
            if (buffer.rfind("Tr ", 0) == 0) {
                std::stringstream ss(buffer.substr(3));
                double transparency;
                ss >> transparency;
                material.getMaterialList()[currentMaterial].setOpacity(1.0 - transparency);
            }
            if (buffer.rfind("Ns ", 0) == 0) {
                std::stringstream ss(buffer.substr(3));
                double shininess;
                ss >> shininess;
                material.getMaterialList()[currentMaterial].setShininess(shininess);
            }
            if (buffer.rfind("map_Kd ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(7));
                std::string textureName;
                size_t lastSlash = fileName.find_last_of("/\\");
                bufferStream >> textureName;
                std::string objDir;
                if (lastSlash != std::string::npos) {
                    objDir = fileName.substr(0, lastSlash + 1);
                } else {
                    objDir = "";
                }

                std::string mtlFullPath = objDir + textureName;
                material.getMaterialList()[currentMaterial].setTexture(Texture(mtlFullPath));
            }
        }
    }

    void ObjFile::parseObjFile(std::string fileName)
    {
        std::fstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return;
            // throw std::runtime_error("Unable to open file");
        }
        std::string buffer;
        std::string currentMaterial = "default";
        while (std::getline(file, buffer)) {
            if (buffer.rfind("mtllib ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(7));
                std::string mtlFileName;
                bufferStream >> mtlFileName;
                    std::string objDir;
                size_t lastSlash = fileName.find_last_of("/\\");
                if (lastSlash != std::string::npos) {
                    objDir = fileName.substr(0, lastSlash + 1);
                } else {
                    objDir = "";
                }

                std::string mtlFullPath = objDir + mtlFileName;
                parseMltFile(mtlFullPath);
            }
            if (buffer.rfind("usemtl ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(7));
                bufferStream >> currentMaterial;
            }
            if (buffer.rfind("vt ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(3));
                double u, v, w = 0.0;
                bufferStream >> u >> v;
                if (!(bufferStream >> w)) w = 0.0;
                textureCoords.push_back(Math::Point3D(u, v, w));
            }

            if (buffer.rfind("v ", 0) == 0) {
                double x, y, z;
                std::stringstream bufferStream(buffer.substr(2));
                bufferStream >> x >> y >> z;
                vertices.push_back(Math::Point3D(x, y, z));
            }
            if (buffer.rfind("f ", 0) == 0) {
                std::stringstream bufferStream(buffer.substr(2));
                std::vector<size_t> faceIndices;
                std::vector<size_t> textureIndices;
                std::string token;

                while (bufferStream >> token) {
                    std::replace(token.begin(), token.end(), '/', ' ');
                    std::stringstream tokenStream(token);
                    int index;
                    int textureIndex;
                    tokenStream >> index;
                    if (tokenStream.peek() == ' ') {
                        tokenStream.ignore();
                        tokenStream >> textureIndex;
                    } else {
                        textureIndex = 0;
                    }
                    faceIndices.push_back(index);
                    textureIndices.push_back(textureIndex);
                }
                for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                    _sides.push_back(Face (
                        {faceIndices[0], faceIndices[i], faceIndices[i + 1]},
                        {textureIndices[0], textureIndices[i], textureIndices[i + 1]},
                        currentMaterial));
                }

            }
        }
        std::cout << "Rendering " << _sides.size() << " polygons\n";
        file.close();
    }

    HitInfo ObjFile::intersect(const Ray& ray) const
    {

        std::vector<RayTracer::HitInfo> hits = tree->intersects(ray);
        if (hits.empty()) {
            return HitInfo();
        }
        double minDistance = std::numeric_limits<double>::infinity();
        HitInfo closestHit;

        for (const auto& hit : hits) {
            if (hit.getDistance() >= 0.0 && hit.getDistance() < minDistance) {
                closestHit = hit;
                minDistance = hit.getDistance();
            }
        }
        return closestHit;
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "objfiles";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return std::make_unique<ObjFile>(params);
        }
    }
}