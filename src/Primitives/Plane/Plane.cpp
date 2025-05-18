/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** Plane.cpp
*/

#include "Plane.hpp"
#include <cmath>
#include <stdexcept>
#include "../../Parsing/ArgumentMap.hpp"
#include "../../Ray/Ray.hpp"

namespace RayTracer {

    Plane::Plane(Axis axis, double position)
        : axis(axis), position(position) {}

    Plane::Plane(ArgumentMap map)
    {
        if (map["axis"].as<std::string>() == "Z")
            axis = Axis::Z;
        if (map["axis"].as<std::string>() == "X")
            axis = Axis::X;
        if (map["axis"].as<std::string>() == "Y")
            axis = Axis::Y;
        position = map["position"].as<double>();
        if (map["material"].exists<std::reference_wrapper<RayTracer::Material>>())
            material = map["material"].as<std::reference_wrapper<RayTracer::Material>>();
        if (map["color"].exists<Color>())
            material._color = map["color"].as<Color>();
    }

    Plane::Plane(const std::vector<double>& params) {
        if (params.size() >= 5) {
            axis = static_cast<Axis>(static_cast<int>(params[0]));
            position = params[1];
        } else {
            axis = Axis::Z;
            position = 0.0;
        }
    }

    HitInfo Plane::intersect(const Ray& ray) const {
        HitInfo info;
        info.hit = false;
        double normalComponent = 0;
        Math::Vector3D normal;
        Math::Vector3D uAxis;
        Math::Vector3D vAxis;

        switch (axis) {
            case Axis::X:
                normal = Math::Vector3D(1, 0, 0);
                uAxis = Math::Vector3D(0, 1, 0);
                vAxis = Math::Vector3D(0, 0, 1);
                normalComponent = ray.getDirection().getX();
                break;
            case Axis::Y:
                normal = Math::Vector3D(0, 1, 0);
                uAxis = Math::Vector3D(1, 0, 0);
                vAxis = Math::Vector3D(0, 0, 1);
                normalComponent = ray.getDirection().getY();
                break;
            case Axis::Z:
                normal = Math::Vector3D(0, 0, 1);
                uAxis = Math::Vector3D(1, 0, 0);
                vAxis = Math::Vector3D(0, 1, 0);
                normalComponent = ray.getDirection().getZ();
                break;
        }

        if (std::fabs(normalComponent) < 1e-8)
            return info;

        double d = 0;
        switch (axis) {
            case Axis::X:
                d = (position - ray.getOrigin().getX()) / ray.getDirection().getX();
                break;
            case Axis::Y:
                d = (position - ray.getOrigin().getY()) / ray.getDirection().getY();
                break;
            case Axis::Z:
                d = (position - ray.getOrigin().getZ()) / ray.getDirection().getZ();
                break;
        }
        if (d < 0)
            return info;
        info.hit = true;
        info.originDirection = ray.getDirection() * -1;
        info.distance = d;
        info.point = ray.getOrigin() + ray.getDirection() * d;
        Math::Point3D planePoint;
        switch (axis) {
            case Axis::X: planePoint = Math::Point3D(position, 0, 0); break;
            case Axis::Y: planePoint = Math::Point3D(0, position, 0); break;
            case Axis::Z: planePoint = Math::Point3D(0, 0, position); break;
        }
        Math::Vector3D local = info.point - planePoint;
        double u = local.dot(uAxis) * material.textureScale;
        double v = local.dot(vAxis) * material.textureScale;

        if (ray.getDirection().dot(normal) > 0)
            info.normal = normal * -1;
        else
            info.normal = normal;
        info.color = material.getColor(u, v);
        info.material = material;
        return info;
    }

    std::string Plane::getName() const {
        return "Plane";
    }

    std::unique_ptr<IPrimitive> Plane::create(const std::vector<double>& params) {
        if (params.size() >= 5) {
            Axis axis = static_cast<Axis>(static_cast<int>(params[0]));
            double position = params[1];

            return std::make_unique<Plane>(axis, position);
        }

        return std::make_unique<Plane>();
    }

    Axis Plane::stringToAxis(const std::string& axisStr) {
        if (axisStr == "X" || axisStr == "x")
            return Axis::X;
        else if (axisStr == "Y" || axisStr == "y")
            return Axis::Y;
        else if (axisStr == "Z" || axisStr == "z")
            return Axis::Z;
        else
            throw std::invalid_argument("Invalid axis string: " + axisStr);
    }

    extern "C" {
        const char *getPrimitiveName() {
            return "planes";
        }
        std::unique_ptr<IPrimitive> createPrimitive(ArgumentMap params) {
            return std::make_unique<Plane>(params);
        }
    }

}