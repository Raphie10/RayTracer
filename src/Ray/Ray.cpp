/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Ray
*/

#include "Ray.hpp"
#include "HitInfo.hpp"
#include "IPrimitive.hpp"
#include "ILights.hpp"
#include "../Builder/Scene.hpp"
#include "../BVHBuilder/BinaryTree.hpp"

namespace RayTracer
{
    Color Ray::computeHitColor(HitInfo &info, const std::shared_ptr<Scene> &scene, int depth) const
    {
        // float diffusionLight = 0.1;
        Color color = (info.color * scene->getAmbientLight());
        for (const auto &light : scene->getLights()) {
            Color lightColor = light->computeLightingColor(info, *scene);
            color += lightColor;
        }
        if (info.material.reflectivity > 0 && depth > 1) {
            Math::Vector3D dir = direction.normalize();
            Math::Vector3D reflectDir = dir - info.normal * 2 * dir.dot(info.normal);
            reflectDir = reflectDir.normalize();
            Ray reflectRay(info.point + info.normal * 0.001, reflectDir);
            Color reflectColor = reflectRay.trace_ray(scene, depth - 1);
            color = color * (1 - info.material.reflectivity) + reflectColor * info.material.reflectivity;
        }
        return color;
    }
    std::vector<HitInfo> Ray::find_intersection(const std::vector<std::unique_ptr<IPrimitive>> &primitives, const std::unique_ptr<Node> &tree) const
    {
        std::vector<HitInfo> hits;
        for (const auto& element : primitives) {
            HitInfo hitInfo = element->intersect(*this);
            if (hitInfo.hit && hitInfo.distance > 1e-6) {
                hits.push_back(hitInfo);
            }
        }
        auto treeIntersect = tree->intersects(*this);
        hits.insert(hits.end(), treeIntersect.begin(), treeIntersect.end());

        std::sort(hits.begin(), hits.end(), [](const HitInfo& a, const HitInfo& b) {
            return a.distance < b.distance;
        });
        return hits;
    }
    Color Ray::trace_ray(const std::shared_ptr<Scene> &scene, int depth)
    {
        if (depth <= 0) {
            return scene->getBackgroundColor();
        }
        auto hits = find_intersection(scene->getPrimitives(), scene->getTree());
        if (hits.size() == 0) {
            return scene->getBackgroundColor();
        }
        double curOpacity = 0.0;
        Color color(0, 0, 0);
        for (auto& hit : hits) {
            Color hitColor = computeHitColor(hit, scene, depth);
            color += hitColor * std::min(hit.material.opacity, (1.0 - curOpacity));
            curOpacity += hit.material.opacity;
            if (curOpacity >= 1.0) {
                break;
            }
        }
        if (curOpacity < 1.0) {
            color += scene->getBackgroundColor() * (1.0 - curOpacity);
        }
        return color;
    }
}
