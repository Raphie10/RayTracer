/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** PointLight
*/

#include "PointLight.hpp"
#include "../../Interfaces/ILights.hpp"
#include <unordered_map>
#include "../../Builder/RayTracer.hpp"
#include "../../Parsing/ArgumentMap.hpp"
#include "../../Interfaces/HitInfo.hpp"

namespace RayTracer {

    PointLight::~PointLight()
    {
    }

    Color PointLight::computeLightingColor(HitInfo &info, const Scene &scene) const
    {
        Math::Vector3D lightDir = position - info.point;

        double distance = lightDir.length();
        lightDir = lightDir.normalize();
        const double bias = 1e-4;
        Ray ray(info.point + info.normal * bias, lightDir);
        auto hits = ray.find_intersection(scene.getPrimitives());
        Color filteredLightColor = _color;
            for (const auto& hit : hits) {
                if (hit.distance >= distance)
                    break;
                if (hit.material.opacity >= 1.0) {
                    return Color(0, 0, 0);
                }
                double transparency = 1.0 - hit.material.opacity;
                filteredLightColor = filteredLightColor * hit.material._color * transparency;

            }
        Math::Vector3D N = info.normal.normalize();
        Math::Vector3D L = lightDir.normalize();
        Math::Vector3D V = (info.originDirection).normalize();
        double diffuse = std::max(N.dot(L), 0.0);
        diffuse *= _intensity;
        double attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);;
        diffuse *= attenuation;
        double shininess = info.material.shininess;
        Math::Vector3D reflectedLightDir = ((N *  ((N.dot( L) * 2) )) - L).normalize();
        Color specularColor = info.color * (_intensity * pow(std::max(reflectedLightDir.dot(V), 0.0), shininess));
        Color color =  ((info.color * diffuse) * filteredLightColor) + specularColor;
        return color;
    }

    extern "C" {
        const char *getLightName() {
            return "point";
        }
        std::unique_ptr<ILights> createLight(ArgumentMap params)
        {
            return  std::make_unique<PointLight>(params);
        }
    }
}
