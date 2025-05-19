/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** HitInfo
*/

#pragma once

#include "Vector.hpp"
#include "Points.hpp"
#include "Color.hpp"
#include "../Materials/Materials.hpp"
namespace RayTracer {
    class HitInfo {
        public:

            HitInfo() : hit(false), distance(0.0), point(), normal(), color(), material(){}
            HitInfo(bool hit, double distance, const Math::Point3D &point,
                    const Math::Vector3D &normal, const Color &color, const Material &material)
                : hit(hit), distance(distance), point(point), normal(normal), color(color), material(material) {}
            bool isHit() const { return hit; }
            void setHit(bool hit) { this->hit = hit; }
            double getDistance() const { return distance; }
            void setDistance(double distance) { this->distance = distance; }
            const Math::Point3D &getPoint() const { return point; }
            void setPoint(const Math::Point3D &point) { this->point = point; }
            const Math::Vector3D &getNormal() const { return normal; }
            void setNormal(const Math::Vector3D &normal) { this->normal = normal; }
            const Math::Vector3D &getOriginDirection() const { return originDirection; }
            void setOriginDirection(const Math::Vector3D &originDirection) { this->originDirection = originDirection; }
            const Color &getColor() const { return color; }
            void setColor(const Color &color) { this->color = color; }
            const Material &getMaterial() const { return material; }
            void setMaterial(const Material &material) { this->material = material; }

        private:
            bool hit = false;
            double distance;
            Math::Point3D point;
            Math::Vector3D normal;
            Math::Vector3D originDirection;
            Color color;
            Material material;
    };
}