/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Triangle
*/

#pragma once

#include "IPrimitive.hpp"
#include "HitInfo.hpp"
#include <memory>
#include "Vector.hpp"
#include "Points.hpp"
#include "../Parsing/ArgumentMap.hpp"
#include "../BVHBuilder/AABB.hpp"
#include <optional>
#include "../Materials/Materials.hpp"

namespace RayTracer {
    class Triangle : public IPrimitive {
        public:
            Triangle();
            Triangle(ArgumentMap params);
            ~Triangle();
            HitInfo intersect(const Ray& ray) const;
            std::string getName() const { return "Triangle"; }
            std::unique_ptr<IPrimitive> create(const std::vector<double>&)
            {
                return std::make_unique<Triangle>();
            }
            AABB getBoundingBox();
        private:
            Math::Point3D vertex1;
            Math::Point3D vertex2;
            Math::Point3D vertex3;
            Math::Vector3D textPoint1;
            Math::Vector3D textPoint2;
            Math::Vector3D textPoint3;
            Material material;
            // Color _color;
            Math::Vector3D edge1;
            Math::Vector3D edge2;
            Math::Vector3D normal;
            std::optional<AABB> boundingBox;
    };
}