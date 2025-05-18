/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** PointLight
*/

#pragma once

#include "../../Vectors/Vector.hpp"
#include "../../Points/Points.hpp"
#include "../ALights.hpp"
#include "../../Interfaces/IPrimitive.hpp"
#include "../../Builder/RayTracer.hpp"

namespace RayTracer {
    class PointLight : public ALights {
        public:
            PointLight(const Math::Point3D& pos, const Color& col, double intensity) : ALights(col, intensity), position(pos) {};
            PointLight(ArgumentMap params) : ALights(params["color"].as<Color>(), params["intensity"].as<double>()), position(Math::Point3D(
                params["position"].as<Math::Point3D>())) {}
            ~PointLight();
            Color computeLightingColor(HitInfo &info, const Scene &scene) const;
        private:
            Math::Point3D position;
    };
}

