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
#include "../../Parsing/ArgumentMap.hpp"

namespace RayTracer {

    class DirectionalLight : public ALights {
        public:
            DirectionalLight(const Math::Vector3D& direction, const Color& col, double intensity) : ALights(col, intensity), _direction(direction){};
            DirectionalLight(ArgumentMap params);
            ~DirectionalLight();
            Color computeLightingColor(HitInfo &info, const Scene &scene) const;
        private:
            Math::Vector3D _direction;
    };
}

