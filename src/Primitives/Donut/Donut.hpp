// filepath: /home/prevost/delivery_tek2/RayTracer/src/Primitives/Donut/Donut.hpp
/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Donut.hpp
*/

#pragma once

#include "IPrimitive.hpp"
#include <memory>
#include "../../Parsing/ArgumentMap.hpp"
#include "Color.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {

    class Donut : public IPrimitive {
        private:
            Math::Point3D center;
            double majorRadius;
            double minorRadius;
            // Color color;
            Math::Vector3D direction;
            Material material;
        public:
            Donut(const Math::Point3D& center = Math::Point3D(0, 0, 0),
                double majorRadius = 2.0,
                double minorRadius = 0.5,
                const Math::Vector3D& direction = Math::Vector3D(0, 1, 0));
            Donut(const std::vector<double>& params);
            Donut(ArgumentMap params);
            HitInfo intersect(const Ray& ray) const override;
            std::string getName() const override;
            std::unique_ptr<IPrimitive> create(const std::vector<double>& params) override;
            AABB getBoundingBox() {
            double boundingRadius = majorRadius + minorRadius;
    
            return AABB(
                Math::Point3D(center.getX() - boundingRadius, center.getY() - boundingRadius, center.getZ() - boundingRadius),
                Math::Point3D(center.getX() + boundingRadius, center.getY() + boundingRadius, center.getZ() + boundingRadius)
            );
            }

            Math::Point3D getCenter() const { return center; }
            double getMajorRadius() const { return majorRadius; }
            double getMinorRadius() const { return minorRadius; }
            Math::Vector3D getDirection() const { return direction; }

            void setCenter(const Math::Point3D& center) { this->center = center; }
            void setMajorRadius(double majorRadius) { this->majorRadius = majorRadius; }
            void setMinorRadius(double minorRadius) { this->minorRadius = minorRadius; }
            void setDirection(const Math::Vector3D& direction) { this->direction = direction.normalize(); }
    };

}

