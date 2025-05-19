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
        Math::Vector3D lightDir = position - info.getPoint();

        double distance = lightDir.length();
        lightDir = lightDir.normalize();
        const double bias = 1e-4;
        Ray ray(info.getPoint() + info.getNormal() * bias, lightDir);
        auto hits = ray.find_intersection(scene.getPrimitives(), scene.getTree());
        Color filteredLightColor = _color;
        for (const auto& hit : hits) {
            if (hit.getDistance() >= distance)
                break;
            if (hit.getMaterial().getOpacity() >= 1.0) {
                return Color(0, 0, 0);
            }
            double transparency = 1.0 - hit.getMaterial().getOpacity();
            filteredLightColor = filteredLightColor * hit.getMaterial().getColor() * transparency;

        }
        Math::Vector3D N = info.getNormal().normalize();
        Math::Vector3D L = lightDir.normalize();
        Math::Vector3D V = (info.getOriginDirection()).normalize();
        double diffuse = std::max(N.dot(L), 0.0);
        diffuse *= _intensity;
        double attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);;
        diffuse *= attenuation;
        double shininess = info.getMaterial().getShininess();
        Math::Vector3D reflectedLightDir = ((N *  ((N.dot( L) * 2) )) - L).normalize();
        Color specularColor = info.getColor() * (_intensity * pow(std::max(reflectedLightDir.dot(V), 0.0), shininess));
        Color color =  ((info.getColor() * diffuse) * filteredLightColor) + specularColor;
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
