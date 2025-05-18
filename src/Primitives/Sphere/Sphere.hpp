/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Sphere.hpp
*/

#pragma once

#include "IPrimitive.hpp"
#include <memory>
#include "../../Parsing/ArgumentMap.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {

    class Sphere : public IPrimitive {
        private:
            Math::Point3D center;
            double radius;
            Material material;
        public:
            Sphere(const Math::Point3D& center = Math::Point3D(0, 0, 0),
                double radius = 1.0);
            Sphere(ArgumentMap params);
            HitInfo intersect(const Ray& ray) const override;
            std::string getName() const override;
            std::unique_ptr<IPrimitive> create(const std::vector<double>& params) override;
            AABB getBoundingBox() {
                return AABB();
            }

            Math::Point3D getCenter() const { return center; }
            double getRadius() const { return radius; }

            void setCenter(const Math::Point3D& center) { this->center = center; }
            void setRadius(double radius) { this->radius = radius; }
        };

}
