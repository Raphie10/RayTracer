/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"
#include "Vector.hpp"

namespace RayTracer
{
    Triangle::Triangle()
    {
    }

    Triangle::Triangle(ArgumentMap params)
    {
        ArgumentMap vertexParam1 = params["vertex1"].as<ArgumentMap>();
        ArgumentMap vertexParam2 = params["vertex2"].as<ArgumentMap>();
        ArgumentMap vertexParam3 = params["vertex3"].as<ArgumentMap>();
        vertex1 = Math::Point3D(
            vertexParam1["x"].as<double>(),
            vertexParam1["y"].as<double>(),
            vertexParam1["z"].as<double>());
        vertex2 = Math::Point3D(
            vertexParam2["x"].as<double>(),
            vertexParam2["y"].as<double>(),
            vertexParam2["z"].as<double>());
        vertex3 = Math::Point3D(
            vertexParam3["x"].as<double>(),
            vertexParam3["y"].as<double>(),
            vertexParam3["z"].as<double>());
        if (params["textureV1"].exists<ArgumentMap>()) {
            ArgumentMap textureParam1 = params["textureV1"].as<ArgumentMap>();
            ArgumentMap textureParam2 = params["textureV2"].as<ArgumentMap>();
            ArgumentMap textureParam3 = params["textureV3"].as<ArgumentMap>();
            textPoint1 = Math::Vector3D(
                textureParam1["x"].as<double>(),
                textureParam1["y"].as<double>(),
                textureParam1["z"].as<double>());
            textPoint2 = Math::Vector3D(
                textureParam2["x"].as<double>(),
                textureParam2["y"].as<double>(),
                textureParam2["z"].as<double>());
            textPoint3 = Math::Vector3D(
                textureParam3["x"].as<double>(),
                textureParam3["y"].as<double>(),
                textureParam3["z"].as<double>());
        } else {
            textPoint1 = Math::Vector3D(0, 0, 0);
            textPoint2 = Math::Vector3D(1, 0, 0);
            textPoint3 = Math::Vector3D(0, 1, 0);
        }
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>()) {
            material._color = params["color"].as<Color>();
        }
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        normal = edge1.cross(edge2).normalize();
    }

    Triangle::~Triangle()
    {
    }

    HitInfo Triangle::intersect(const Ray& ray) const
    {
        HitInfo info;
        info.hit = false;

        Math::Vector3D P = ray.getDirection().cross(edge2);
        auto det = edge1.dot(P);
        if (det < 1e-8f)
            return info;
        // if (std::fabs(det) < 1e-8f) {
        //     return info;
        // }
        double f = 1 / det;
        Math::Vector3D S = ray.getOrigin() - vertex1;
        double u = (S.dot(P)) * f;
        if (u < 0 || u > 1)
            return info;
        Math::Vector3D Q = S.cross(edge1);
        double v = f * (ray.getDirection().dot(Q));
        if (v < 0 || u + v > 1)
            return info;
        double hitDistance = f * (edge2.dot(Q));
        if (hitDistance < 1e-8f)
            return info;
        info.hit = true;
        info.originDirection = ray.getDirection() * -1;
        info.distance = hitDistance;
        info.normal = normal;
        info.point = ray.getOrigin() + ray.getDirection() * hitDistance;
        double w = 1.0 - u - v;
        double texU = textPoint1.getX() * w + textPoint2.getX() * u + textPoint3.getX() * v;
        double texV = textPoint1.getY() * w + textPoint2.getY() * u + textPoint3.getY() * v;
        info.color = material.getColor(texU, texV);
        info.material = material;
        return info;
    }

    AABB Triangle::getBoundingBox()
    {
        if (boundingBox.has_value())
            return boundingBox.value();
        double minX = std::min(std::min(vertex1.getX(), vertex2.getX()), vertex3.getX());
        double minY = std::min(std::min(vertex1.getY(), vertex2.getY()), vertex3.getY());
        double minZ = std::min(std::min(vertex1.getZ(), vertex2.getZ()), vertex3.getZ());
        Math::Point3D minPoint(minX, minY, minZ);
        double maxX = std::max(std::max(vertex1.getX(), vertex2.getX()), vertex3.getX());
        double maxY = std::max(std::max(vertex1.getY(), vertex2.getY()), vertex3.getY());
        double maxZ = std::max(std::max(vertex1.getZ(), vertex2.getZ()), vertex3.getZ());
        Math::Point3D maxPoint(maxX, maxY, maxZ);
        boundingBox = AABB(minPoint, maxPoint);
        return (boundingBox.value());
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "triangles";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return  std::make_unique<Triangle>(params);
        }
    }
}