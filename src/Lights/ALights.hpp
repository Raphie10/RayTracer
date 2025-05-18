/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ALights
*/

#pragma once

#include "../Vectors/Vector.hpp"
#include "../Points/Points.hpp"
#include "../Interfaces/ILights.hpp"
#include "../Interfaces/IPrimitive.hpp"
#include "Color.hpp"

namespace RayTracer {

    class ALights : public ILights {
        public:
            ALights(const Color& col, double intensity) : _color(col), _intensity(intensity) {}
            ~ALights();
            Color getColor() const;
            double getIntensity() const;
            virtual Color computeLightingColor(HitInfo &info, const Scene &scene) const = 0;
        protected:
            Color _color;
            double _intensity;
    };

}
