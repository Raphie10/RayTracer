/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Cone.cpp
*/

#include "Cone.hpp"
#include <cmath>

namespace RayTracer {

    Cone::Cone(const Math::Point3D& position, double radius, double height,
               const Math::Vector3D& direction)
        : position(position), radius(radius), height(height),
          direction(direction.normalize()) {}

    Cone::Cone(const std::vector<double>& params) {
        position = Math::Point3D(params[0], params[1], params[2]);
        radius = params[3];
        height = params[4];
        direction = Math::Vector3D(params[5], params[6], params[7]).normalize();
    }

    Cone::Cone(ArgumentMap params)
    {
        position = params["position"].as<Math::Point3D>();
        radius = params["radius"].as<double>();
        height = params["height"].as<double>();
        ArgumentMap directionMap = params["direction"].as<ArgumentMap>();
        direction = Math::Vector3D(directionMap["x"].as<double>(), directionMap["y"].as<double>(), directionMap["z"].as<double>());
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>())
            material._color = params["color"].as<Color>();
    }

    HitInfo Cone::intersect(const Ray& ray) const
    {
        HitInfo info;
        info.hit = false;
        Math::Vector3D axis = direction.normalize();
        Math::Vector3D oc = ray.getOrigin() - position;
        double tanTheta = radius / height;
        double tanTheta2 = tanTheta * tanTheta;
        double a = ray.getDirection().dot(ray.getDirection()) - (1 + tanTheta2) * pow(ray.getDirection().dot(axis), 2);
        double b = 2 * (oc.dot(ray.getDirection()) - (1 + tanTheta2) * (oc.dot(axis)) * (ray.getDirection().dot(axis)));
        double c = oc.dot(oc) - (1 + tanTheta2) * pow(oc.dot(axis), 2);
        double discriminant = b * b - 4 * a * c;
        double tSide = INFINITY;
        bool hitSide = false;
        if (discriminant >= 0) {
            double t1 = (-b - sqrt(discriminant)) / (2 * a);
            double t2 = (-b + sqrt(discriminant)) / (2 * a);
            double t = (t1 > 0) ? t1 : ((t2 > 0) ? t2 : -1);
            if (t > 0) {
                Math::Point3D hitPoint = ray.getOrigin() + ray.getDirection() * t;
                Math::Vector3D ap = hitPoint - position;
                double heightOnCone = ap.dot(axis);
                if (heightOnCone >= 0 && heightOnCone <= height) {
                    tSide = t;
                    hitSide = true;
                }
            }
        }
        double tBase = INFINITY;
        bool hitBase = false;
        Math::Vector3D baseNormal;
        Math::Point3D baseCenter = position + axis * height;
        double denom = ray.getDirection().dot(axis);
        if (fabs(denom) > 1e-6) {
            double t = (baseCenter - ray.getOrigin()).dot(axis) / denom;
            if (t > 0) {
                Math::Point3D hitPoint = ray.getOrigin() + ray.getDirection() * t;
                Math::Vector3D baseToHit = hitPoint - baseCenter;
                double distanceFromCenter = baseToHit.length();
                if (distanceFromCenter <= radius) {
                    tBase = t;
                    hitBase = true;
                    baseNormal = axis * (-1.0);
                }
            }
        }
        if (!hitSide && !hitBase)
            return info;
        if (hitSide && (!hitBase || tSide <= tBase)) {
            info.hit = true;
            info.distance = tSide;
            info.originDirection = ray.getDirection() * -1;
            info.point = ray.getOrigin() + ray.getDirection() * tSide;
            Math::Vector3D ap = info.point - position;
            double heightOnCone = ap.dot(axis);
            Math::Vector3D s = info.point - (position + axis * heightOnCone);
            info.normal = (s - axis * s.dot(axis) / axis.dot(axis)).normalize();
            double v = heightOnCone / height;
            Math::Vector3D hitVec = info.point - position;
            Math::Vector3D radial = hitVec - axis * heightOnCone;
            double theta = atan2(radial.getZ(), radial.getX());
            double u = (theta + M_PI) / (2 * M_PI);
            material.getColor(u, v);
        } else {
            info.hit = true;
            info.originDirection = ray.getDirection() * -1;
            info.distance = tBase;
            info.point = ray.getOrigin() + ray.getDirection() * tBase;
            info.normal = baseNormal;
            Math::Vector3D local = info.point - baseCenter;
            double u = 0.5 + local.getX() / (2.0 * radius);
            double v = 0.5 + local.getZ() / (2.0 * radius);
            info.color = material.getColor(u, v, "bottom");
        }
        info.color = material._color;
        info.material = material;
        return info;
    }

    std::string Cone::getName() const
    {
        return "Cone";
    }

    std::unique_ptr<IPrimitive> Cone::create(const std::vector<double>& params)
    {
        if (params.size() >= 11) {
            Math::Point3D position(params[0], params[1], params[2]);
            double radius = params[3];
            double height = params[4];
            Math::Vector3D direction(params[5], params[6], params[7]);
            // Color color(params[8], params[9], params[10]);
            // double reflection = (params.size() > 11) ? params[11] : 0.0;
            return std::make_unique<Cone>(position, radius, height, direction);
        }
        return std::make_unique<Cone>(params);
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "cones";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return std::make_unique<Cone>(params);
        }
    }

}