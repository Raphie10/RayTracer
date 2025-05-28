/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Donut.cpp
*/

#include "Donut.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include "../Ray/Ray.hpp"
#include "../../Parsing/ArgumentMap.hpp"

namespace RayTracer {

    Donut::Donut(const Math::Point3D& center, double majorRadius, double minorRadius, const Math::Vector3D& direction)
        : center(center), majorRadius(majorRadius), minorRadius(minorRadius), direction(direction.normalize()) {}

    Donut::Donut(const std::vector<double>& params) :
        center(params[0], params[1], params[2]),
        majorRadius(params[3]),
        minorRadius(params[4]),
        direction(params[9], params[10], params[11])
    {
        direction = direction.normalize();
    }


    Donut::Donut(ArgumentMap params)
    {
        center = params["position"].as<Math::Point3D>();
        majorRadius = params["major_radius"].as<double>();
        minorRadius = params["minor_radius"].as<double>();
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>())
            material.setColor(params["color"].as<Color>());
        ArgumentMap directionMap = params["direction"].as<ArgumentMap>();
        direction = Math::Vector3D(directionMap["x"].as<double>(), directionMap["y"].as<double>(), directionMap["z"].as<double>());
    }

    HitInfo Donut::intersect(const Ray& ray) const
    {
        HitInfo info;
        info.setHit(false);

        Math::Point3D newOrigin(
            ray.getOrigin().getX() - center.getX(),
            ray.getOrigin().getY() - center.getY(),
            ray.getOrigin().getZ() - center.getZ()
        );
        Math::Vector3D up = direction;
        Math::Vector3D right, forward;
        if (std::abs(up.getX()) < 0.9)
            right = Math::Vector3D(1, 0, 0).cross(up).normalize();
        else
            right = Math::Vector3D(0, 1, 0).cross(up).normalize();
        forward = up.cross(right).normalize();
        Math::Point3D transformedOrigin(
            newOrigin.getX() * right.getX() + newOrigin.getY() * right.getY() + newOrigin.getZ() * right.getZ(),
            newOrigin.getX() * up.getX() + newOrigin.getY() * up.getY() + newOrigin.getZ() * up.getZ(),
            newOrigin.getX() * forward.getX() + newOrigin.getY() * forward.getY() + newOrigin.getZ() * forward.getZ()
        );
        Math::Vector3D transformedDirection(
            ray.getDirection().getX() * right.getX() + ray.getDirection().getY() * right.getY() + ray.getDirection().getZ() * right.getZ(),
            ray.getDirection().getX() * up.getX() + ray.getDirection().getY() * up.getY() + ray.getDirection().getZ() * up.getZ(),
            ray.getDirection().getX() * forward.getX() + ray.getDirection().getY() * forward.getY() + ray.getDirection().getZ() * forward.getZ()
        );
        Math::Vector3D normalizedTransformedDirection = transformedDirection.normalize();

        double R = majorRadius;
        double r = minorRadius;
        double R2 = R * R;
        double r2 = r * r;

        Math::Vector3D d = normalizedTransformedDirection;
        Math::Vector3D o(transformedOrigin.getX(), transformedOrigin.getY(), transformedOrigin.getZ());

        double a = d.dot(d);
        double b = 2.0 * o.dot(d);
        double c = o.dot(o) - R2 - r2;
        double a4 = a * a;
        double a3 = 2.0 * a * b;
        double a2 = b * b + 2.0 * a * c + 4.0 * R2 * d.getY() * d.getY();
        double a1 = 2.0 * b * c + 8.0 * R2 * o.getY() * d.getY();
        double a0 = c * c + 4.0 * R2 * (o.getY() * o.getY() - r2);

        a3 /= a4;
        a2 /= a4;
        a1 /= a4;
        a0 /= a4;
        a4 = 1.0;

        std::vector<double> roots;
        const double epsilon = 1e-6;
        const int maxIterations = 100;

        std::vector<double> initialGuesses = {-10.0, -1.0, 0.0, 1.0, 10.0};
        for (double guess : initialGuesses) {
            double t = guess;
            bool converged = false;
            for (int i = 0; i < maxIterations; ++i) {
                double P = a4*t*t*t*t + a3*t*t*t + a2*t*t + a1*t + a0;
                double Pprime = 4.0*a4*t*t*t + 3.0*a3*t*t + 2.0*a2*t + a1;
                if (std::abs(Pprime) < epsilon)
                    break;
                double t_new = t - P / Pprime;
                if (std::abs(t_new - t) < epsilon) {
                    converged = true;
                    t = t_new;
                    break;
                }
                t = t_new;
            }
            if (converged && t > epsilon) {
                bool isNew = true;
                for (double root : roots) {
                    if (std::abs(root - t) < epsilon) {
                        isNew = false;
                        break;
                    }
                }
                if (isNew)
                    roots.push_back(t);
            }
        }
        if (roots.empty())
            return info;
        std::sort(roots.begin(), roots.end());
        const double shadowEpsilon = 1e-3;
        double t = -1;
        for (double root : roots) {
            if (root > shadowEpsilon) {
                t = root;
                break;
            }
        }
        if (t < 0)
            return info;
        Math::Point3D hitPoint = transformedOrigin + normalizedTransformedDirection * t;
        double x = hitPoint.getX();
        double y = hitPoint.getY();
        double z = hitPoint.getZ();

        double dist = std::sqrt(x*x + z*z);
        double diff = std::sqrt((dist - R)*(dist - R) + y*y) - r;

        if (std::abs(diff) > 5e-3)
            return info;
        info.setHit(true);
        info.setDistance(t);
        info.setPoint(ray.getOrigin() + ray.getDirection() * t);
        Math::Point3D localHitPoint = hitPoint;
        double distToRing = std::sqrt(localHitPoint.getX()*localHitPoint.getX() + localHitPoint.getZ()*localHitPoint.getZ());
        Math::Vector3D ringToHit;
        if (distToRing > 1e-6) {
            double ringX = localHitPoint.getX() * R / distToRing;
            double ringZ = localHitPoint.getZ() * R / distToRing;
            ringToHit = Math::Vector3D(
                localHitPoint.getX() - ringX,
                localHitPoint.getY(),
                localHitPoint.getZ() - ringZ
            );
            double dotProduct = ringToHit.dot(normalizedTransformedDirection);
            if (dotProduct > 0) {
                ringToHit = ringToHit * (-1.0);
            }
        } else {
            ringToHit = Math::Vector3D(0, localHitPoint.getY() > 0 ? 1 : -1, 0);
            if (normalizedTransformedDirection.getY() * ringToHit.getY() > 0) {
                ringToHit.setY(-ringToHit.getY());
            }
        }
        Math::Vector3D normalizedRingToHit = ringToHit.normalize();
        Math::Vector3D globalNormal(
            normalizedRingToHit.getX() * right.getX() + normalizedRingToHit.getY() * up.getX() + normalizedRingToHit.getZ() * forward.getX(),
            normalizedRingToHit.getX() * right.getY() + normalizedRingToHit.getY() * up.getY() + normalizedRingToHit.getZ() * forward.getY(),
            normalizedRingToHit.getX() * right.getZ() + normalizedRingToHit.getY() * up.getZ() + normalizedRingToHit.getZ() * forward.getZ()
        );
        double theta = atan2(localHitPoint.getZ(), localHitPoint.getX());
        double u = (theta + M_PI) / (2 * M_PI);
        double distToCenterXZ = std::sqrt(localHitPoint.getX() * localHitPoint.getX() + localHitPoint.getZ() * localHitPoint.getZ());
        double ringX = localHitPoint.getX() * majorRadius / distToCenterXZ;
        double ringZ = localHitPoint.getZ() * majorRadius / distToCenterXZ;

        double dx = localHitPoint.getX() - ringX;
        double dy = localHitPoint.getY();
        double dz = localHitPoint.getZ() - ringZ;

        double phi = atan2(dy, std::sqrt(dx * dx + dz * dz));
        double v = (phi + M_PI / 2) / M_PI;
        info.setNormal(globalNormal.normalize());
        info.setColor(material.getColor(u, v));
        info.setMaterial(material);
        info.setOriginDirection(ray.getDirection() * -1);
        return info;
    }

    std::string Donut::getName() const
    {
        return "Donut";
    }

    std::unique_ptr<IPrimitive> Donut::create(const std::vector<double>& params)
    {
        if (params.size() >= 8) {
            Math::Point3D center(params[0], params[1], params[2]);
            double majorRadius = params[3];
            double minorRadius = params[4];

            return std::make_unique<Donut>(center, majorRadius, minorRadius);
        }

        return std::make_unique<Donut>();
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "donuts";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return std::make_unique<Donut>(params);
        }
    }

}

