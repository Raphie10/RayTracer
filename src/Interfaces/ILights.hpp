/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** ILights.hpp
*/

#pragma once
#include <memory>
#include "Vector.hpp"
#include "Points.hpp"
#include "../Builder/Scene.hpp"
#include "IPrimitive.hpp"

namespace RayTracer {

    class ILights {
        private:

        public:
            ILights() = default;
            virtual ~ILights() = default;

            virtual Color getColor() const = 0;
            virtual double getIntensity() const = 0;
            virtual Color computeLightingColor(HitInfo &info, const Scene &scene) const = 0;
    };

}