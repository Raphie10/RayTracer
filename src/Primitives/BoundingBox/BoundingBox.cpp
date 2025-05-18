/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** BoundingBox
*/

#include "BoundingBox.hpp"
#include "ArgumentMap.hpp"
#include "Points.hpp"

namespace RayTracer
{
    BoundingBox::BoundingBox()
    {
    }

    BoundingBox::BoundingBox(ArgumentMap params)
    {
        ArgumentMap minParam = params["min"].as<ArgumentMap>();
        auto min = Math::Point3D(
            minParam["x"].as<double>(),
            minParam["y"].as<double>(),
            minParam["z"].as<double>()
        );
        ArgumentMap maxParam = params["max"].as<ArgumentMap>();
        auto max = Math::Point3D(
            maxParam["x"].as<double>(),
            maxParam["y"].as<double>(),
            maxParam["z"].as<double>()
        );
        axisAlignedBoundingBox = AABB(max, min);
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
        material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>())
            material._color = params["color"].as<Color>();
    }

    BoundingBox::~BoundingBox()
    {
    }

    HitInfo BoundingBox::intersect(const Ray& ray) const
    {
        HitInfo hitinfo = axisAlignedBoundingBox.getHitInfo(ray, material);

        return hitinfo;
    }

    extern "C" {
        const char *getPrimitiveName()
        {
            return "boundingboxes";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params)
        {
            return std::make_unique<BoundingBox>(params);
        }
    }

}