/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Ray
*/

#pragma once

#include "../Vectors/Vector.hpp"
#include "../Points/Points.hpp"
#include "../Interfaces/HitInfo.hpp"
#include "../Interfaces/IPrimitive.hpp"
// #include "../Builder/Scene.hpp"
#include <vector>
#include <memory>

namespace RayTracer {

    class IPrimitive;
    class ILights;
    class Ray {
        public:

            Ray() = default;
            Ray(const Math::Point3D& o, const Math::Vector3D& d) : origin(o), direction(d) {}
            std::vector<HitInfo> find_intersection(const std::vector<std::unique_ptr<IPrimitive>> &primitives) const;
            Color trace_ray(const class Scene& scene, int depth);
            Color computeHitColor(HitInfo &info, const class Scene &scene, int depth) const;
            const Math::Point3D getOrigin() const { return origin; }
            const Math::Vector3D getDirection() const { return direction; }
        private:
            Math::Point3D origin;
            Math::Vector3D direction;
    };

}
