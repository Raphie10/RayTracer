/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ObjFile
*/

#pragma once

#include "ArgumentMap.hpp"
#include "../DynamicLibrary/DynamicLibrary.hpp"
#include <vector>
#include "Points.hpp"
#include <memory>
#include "../../Interfaces/IPrimitive.hpp"
#include "../../BVHBuilder/AABB.hpp"
#include "../../BVHBuilder/BinaryTree.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {
    class ObjFile : public IPrimitive {
        public:
            ObjFile(ArgumentMap params);
            ~ObjFile();
            void parseObjFile(std::string fileName);
            std::vector<std::unique_ptr<IPrimitive>>  generateTriangles();
            HitInfo intersect(const Ray& ray) const;
            void parseMltFile(std::string fileName);
            std::string getName() const
            {
                return "ObjFile";
            }
            std::unique_ptr<IPrimitive> create(const std::vector<double> &)
            {
                return std::unique_ptr<IPrimitive>();
            }
            AABB getBoundingBox() {
                return tree->getBoundingBox();
            }

            class Face {
                public:
                    Face(const std::vector<size_t> &vertices, const std::vector<size_t> &textureCoords, const std::string &materialName)
                        : vertices(vertices), textureCoords(textureCoords), materialName(materialName) {}
                    std::vector<size_t> getVertices() const { return vertices; }
                    std::vector<size_t> getTextureCoords() const { return textureCoords; }
                    std::string getMaterialName() const { return materialName; }
                private:
                    std::vector<size_t> vertices;
                    std::vector<size_t> textureCoords;
                    std::string materialName;
            };
        private:
            std::string fileName;
            DynamicLibrary triangleLib;
            std::vector<Math::Point3D> vertices;
            std::vector<Math::Point3D> textureCoords = {};
            std::vector<Face> _sides;
            std::vector<Face> textureSides;
            std::unique_ptr<Node> tree;
            // Color _color;
            Math::Point3D _position;
            Material material;
    };
}
