/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** AABB
*/

#pragma once

#include "../Ray/Ray.hpp"
#include "../Materials/Materials.hpp"

namespace RayTracer {
    class Ray;
    class HitInfo;

    class AABB {
        public:
            AABB();
            AABB(Math::Point3D min, Math::Point3D max) : _min(min), _max(max) {
                centroid = (min + max) * 0.5;
            }
            ~AABB();

            HitInfo getHitInfo(const Ray& ray, const Material &material) const;
            bool calculateEntryPoint(const Ray& ray) const;
            AABB operator+(AABB other)
            {
                Math::Point3D min = Math::Point3D (
                    std::min(other._min.getX(), this->_min.getX()),
                    std::min(other._min.getY(), this->_min.getY()),
                    std::min(other._min.getZ(), this->_min.getZ())
                );
                Math::Point3D max = Math::Point3D (
                    std::max(other._max.getX(), this->_max.getX()),
                    std::max(other._max.getY(), this->_max.getY()),
                    std::max(other._max.getZ(), this->_max.getZ())
                );
                return AABB(min, max);
            }
            const Math::Point3D &getCentroid() const {return centroid;}
            const Math::Point3D &getMin() const {return _min;}
            const Math::Point3D &getMax() const {return _max;}
        private:
            Math::Point3D _min;
            Math::Point3D _max;
            Math::Point3D centroid;
    };
}
