/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** HitInfo
*/

#pragma once

#include "Vector.hpp"
#include "Points.hpp"
#include "Color.hpp"
#include "../Materials/Materials.hpp"
namespace RayTracer {
    class HitInfo {
        public:
            bool hit = false;
            double distance;
            Math::Point3D point;
            Math::Vector3D normal;
            Math::Vector3D originDirection;
            Color color;
            Material material;

            HitInfo() : hit(false), distance(0.0), point(), normal(), color(), material(){}
    };
}