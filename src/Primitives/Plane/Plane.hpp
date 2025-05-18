/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Plane.hpp
*/

#pragma once

#include "../../Interfaces/IPrimitive.hpp"
#include <memory>
#include <string>
#include "../Materials/Materials.hpp"

namespace RayTracer {

    enum class Axis {
        X, Y, Z
    };

    class Plane : public IPrimitive {
        private:
            Axis axis;
            double position;
            Material material;
        public:
            Plane(Axis axis = Axis::Z,
                  double position = 0.0);
            Plane(const std::vector<double>& params);
            Plane(ArgumentMap map);
            HitInfo intersect(const Ray& ray) const override;
            std::string getName() const override;
            std::unique_ptr<IPrimitive> create(const std::vector<double>& params) override;
            AABB getBoundingBox() {
                return AABB();
            }

            // Getters
            Axis getAxis() const { return axis; }
            double getPosition() const { return position; }

            // Setters
            void setAxis(Axis axis) { this->axis = axis; }
            void setPosition(double position) { this->position = position; }

            static Axis stringToAxis(const std::string& axisStr);
    };

}
