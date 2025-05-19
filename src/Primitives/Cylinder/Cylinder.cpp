/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Cylinder.cpp
*/

#include "Cylinder.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace RayTracer {

    Cylinder::Cylinder(const Math::Point3D& position, double radius, double height,
        const Math::Vector3D& direction)
        : position(position), radius(radius), height(height),
        direction(direction.normalize()) {}

    Cylinder::Cylinder(const std::vector<double>& params) {
        position = Math::Point3D(params[0], params[1], params[2]);
        radius = params[3];
        height = params[4];
        direction = Math::Vector3D(params[5], params[6], params[7]).normalize();
    }

    Cylinder::Cylinder(ArgumentMap params)
    {
        position = params["position"].as<Math::Point3D>();
        radius = params["radius"].as<double>();
        height = params["height"].as<double>();
        ArgumentMap directionMap = params["direction"].as<ArgumentMap>();
        direction = Math::Vector3D(directionMap["x"].as<double>(), directionMap["y"].as<double>(), directionMap["z"].as<double>());
        if (params["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = params["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (params["color"].exists<Color>())
            material.setColor(params["color"].as<Color>());
    }

    bool intersectCap(const Ray &ray,
        const Math::Point3D &center,
        const Math::Vector3D &normal,
        double radius,
        const Material &material,
        const std::string &faceName,
        HitInfo &info)
    {
        Math::Vector3D oc = ray.getOrigin() - center;
        double denom = ray.getDirection().dot(normal);
        
        if (std::abs(denom) < 1e-6)
            return false;

        double t = -oc.dot(normal) / denom;
        if (t < 1e-6)
            return false;

        Math::Point3D hitPoint = ray.getOrigin() + ray.getDirection() * t;
        Math::Vector3D offset = hitPoint - center;
        Math::Vector3D projectedOffset = offset - normal * offset.dot(normal);
        double dist2 = projectedOffset.dot(projectedOffset);
        
        if (dist2 > radius * radius)
            return false;

        info.setHit(true);
        info.setDistance(t);
        info.setPoint(hitPoint);
        info.setNormal(normal);
        info.setMaterial(material);
        info.setOriginDirection(ray.getDirection() * -1);
        Math::Vector3D u_axis, v_axis;
        if (std::abs(normal.getX()) < std::abs(normal.getY()))
            u_axis = Math::Vector3D(0, normal.getZ(), -normal.getY()).normalize();
        else
            u_axis = Math::Vector3D(normal.getZ(), 0, -normal.getX()).normalize();
        v_axis = normal.cross(u_axis);
        double u = 0.5 + (offset.dot(u_axis) / (2.0 * radius));
        double v = 0.5 + (offset.dot(v_axis) / (2.0 * radius));
        info.setColor(material.getColor(u, v, faceName));
        return true;
    }


    HitInfo Cylinder::intersect(const Ray& ray) const
    {
        HitInfo info;
        info.setHit(false);
        const Math::Vector3D cylinderAxis = direction;
        Math::Vector3D oc = ray.getOrigin() - position;
        double a = ray.getDirection().dot(ray.getDirection()) - std::pow(ray.getDirection().dot(cylinderAxis), 2);
        double b = 2.0 * (oc.dot(ray.getDirection()) - (oc.dot(cylinderAxis) * ray.getDirection().dot(cylinderAxis)));
        double c = oc.dot(oc) - std::pow(oc.dot(cylinderAxis), 2) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return info;
        double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);
        if (t1 < 0 && t2 < 0)
            return info;
        double t = (t1 < 0) ? t2 : ((t2 < 0) ? t1 : std::min(t1, t2));
        Math::Point3D intersectionPoint = ray.getOrigin() + ray.getDirection() * t;
        double projectionOnAxis = (intersectionPoint - position).dot(cylinderAxis);

        if (projectionOnAxis < 0 || projectionOnAxis > height) {
            if (intersectCap(ray, position, cylinderAxis * -1, radius, material, "bottom", info))
                return info;

            Math::Point3D topCenter = position + cylinderAxis * height;
            if (intersectCap(ray, topCenter, cylinderAxis, radius, material, "top", info))
                return info;
            return info;
        }
        info.setHit(true);
        info.setOriginDirection(ray.getDirection() * -1);
        info.setDistance(t);
        info.setMaterial(material);
        info.setPoint(intersectionPoint);
        Math::Vector3D projectionOnAxisVector = cylinderAxis * projectionOnAxis;
        Math::Vector3D centerToIntersect = intersectionPoint - (position + projectionOnAxisVector);
        Math::Vector3D hitVec = info.getPoint() - position;
        double heightOnAxis = hitVec.dot(direction);
        double v = heightOnAxis / height;

        Math::Vector3D radial = hitVec - direction * heightOnAxis;
        double theta = atan2(radial.getZ(), radial.getX());
        double u = (theta + M_PI) / (2 * M_PI);
        info.setNormal(centerToIntersect.normalize());
        info.setColor(material.getColor(u, v));
        return info;
    }

    std::string Cylinder::getName() const
    {
        return "Cylinder";
    }

    std::unique_ptr<IPrimitive> Cylinder::create(const std::vector<double>& params)
    {
        return std::make_unique<Cylinder>(params);
    }

    AABB Cylinder::getBoundingBox() {
    Math::Point3D p1 = position;
    Math::Point3D p2 = position + direction * height;
    
    double minX = std::min(p1.getX(), p2.getX());
    double minY = std::min(p1.getY(), p2.getY());
    double minZ = std::min(p1.getZ(), p2.getZ());
    double maxX = std::max(p1.getX(), p2.getX());
    double maxY = std::max(p1.getY(), p2.getY());
    double maxZ = std::max(p1.getZ(), p2.getZ());
    
    return AABB(
        Math::Point3D(minX - radius, minY - radius, minZ - radius),
        Math::Point3D(maxX + radius, maxY + radius, maxZ + radius)
    );
}

    extern "C" {
        const char *getPrimitiveName() {
            return "cylinders";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return std::make_unique<Cylinder>(params);
        }
    }

}