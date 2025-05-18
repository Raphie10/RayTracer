/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Sphere.cpp
*/

#include "Sphere.hpp"
#include <cmath>
#include "../../Parsing/ArgumentMap.hpp"

namespace RayTracer {

    Sphere::Sphere(const Math::Point3D& center, double radius)
        : center(center), radius(radius) {}

    Sphere::Sphere(ArgumentMap params)
    {
        center = Math::Point3D(params["x"].as<double>(), params["y"].as<double>(), params["z"].as<double>());
        radius = params["r"].as<double>();
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>()) {
            material._color = params["color"].as<Color>();
        }

    }

    HitInfo Sphere::intersect(const Ray& ray) const
    {
        HitInfo info;
        info.hit = false;
        Math::Vector3D oc = ray.getOrigin() - center;
        double a = ray.getDirection().dot(ray.getDirection());
        double b = 2.0 * oc.dot(ray.getDirection());
        double c = oc.dot(oc) - radius * radius;

        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return info;

        double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
        double t = (t1 > 0) ? t1 : t2;
        if (t < 0)
            return info;
        double u, v;
        info.hit = true;
        info.originDirection = ray.getDirection() * -1;
        info.distance = t;
        info.point = ray.getOrigin() + ray.getDirection() * t;
        info.normal = (info.point - center).normalize();
        Math::Vector3D p = (info.point - center).normalize();
        u = 0.5f - atan2(p.getZ(), p.getX()) / (2 * M_PI);
        v = 0.5f + asin(p.getY()) / M_PI;
        info.color = material.getColor(u, v);
        info.material = material;
        return info;
    }

    std::string Sphere::getName() const
    {
        return "Sphere";
    }

    std::unique_ptr<IPrimitive> Sphere::create(const std::vector<double>& params)
    {
        if (params.size() >= 7) {
            Math::Point3D center(params[0], params[1], params[2]);
            double radius = params[3];

            return std::make_unique<Sphere>(center, radius);
        }

        return std::make_unique<Sphere>();
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "spheres";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return  std::make_unique<Sphere>(params);
        }
    }

}
