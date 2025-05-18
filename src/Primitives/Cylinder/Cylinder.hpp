/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Cylinder.hpp
*/

#pragma once

#include "../../Interfaces/IPrimitive.hpp"
#include <memory>
#include "../../Parsing/ArgumentMap.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {

    class Cylinder : public IPrimitive {
        private:
            Math::Point3D position;
            double radius;
            double height;
            Math::Vector3D direction;
            Material material;
        public:
            Cylinder(const Math::Point3D& position = Math::Point3D(0, 0, 0), double radius = 1.0, double height = 1.0,
                const Math::Vector3D& direction = Math::Vector3D(0, 1, 0));
            Cylinder(const std::vector<double>& params);
            Cylinder(ArgumentMap params);
            ~Cylinder() = default;

            HitInfo intersect(const Ray& ray) const override;
            std::string getName() const override;
            std::unique_ptr<IPrimitive> create(const std::vector<double>& params) override;
            AABB getBoundingBox() {
                return AABB(
                    Math::Point3D(position.getX() - radius, position.getY(), position.getZ() - radius),
                    Math::Point3D(position.getX() + radius, position.getY() + height, position.getZ() + radius)
                );
            }

            // Getters
            Math::Point3D getPosition() const { return position; }
            double getRadius() const { return radius; }
            double getHeight() const { return height; }
            Math::Vector3D getDirection() const { return direction; }

            // Setters
            void setPosition(const Math::Point3D& position) { this->position = position; }
            void setRadius(double radius) { this->radius = radius; }
            void setHeight(double height) { this->height = height; }
            void setDirection(const Math::Vector3D& direction) { this->direction = direction.normalize(); }
    };
}