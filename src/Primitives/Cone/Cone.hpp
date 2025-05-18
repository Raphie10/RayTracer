/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Cone.hpp
*/

#pragma once

#include "../../Interfaces/IPrimitive.hpp"
#include <memory>
#include "../../Parsing/ArgumentMap.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {

    class Cone : public IPrimitive {
        private:
            Math::Point3D position;
            double radius;
            double height;
            Math::Vector3D direction;
            // Color color;
            // double reflection;
            Material material;
        public:
            Cone(const Math::Point3D& position = Math::Point3D(0, 0, 0), double radius = 1.0, double height = 1.0,
                const Math::Vector3D& direction = Math::Vector3D(0, -1, 0));
            Cone(const std::vector<double>& params);
            Cone(ArgumentMap params);
            HitInfo intersect(const Ray& ray) const override;
            std::string getName() const override;
            std::unique_ptr<IPrimitive> create(const std::vector<double>& params) override;
            AABB getBoundingBox() {
                return AABB();
            }

            // Getters
            Math::Point3D getposition() const { return position; }
            double getRadius() const { return radius; }
            double getHeight() const { return height; }
            Math::Vector3D getDirection() const { return direction; }
            // Color getColor() const { return color; }
            // double getReflection() const { return reflection; }

            // Setters
            void setposition(const Math::Point3D& position) { this->position = position; }
            void setRadius(double radius) { this->radius = radius; }
            void setHeight(double height) { this->height = height; }
            void setDirection(const Math::Vector3D& direction) { this->direction = direction.normalize(); }
            // void setColor(const Color& color) { this->color = color; }
            // void setReflection(double reflection) { this->reflection = reflection; }
    };

}