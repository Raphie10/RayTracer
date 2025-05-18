/*
** EPITECH PROJECT, 2025
** $
** File description:
** AABB
*/

#include "AABB.hpp"
#include "../Ray/Ray.hpp"
#include <iostream>
#include "HitInfo.hpp"

namespace RayTracer {

    AABB::AABB() : _min(0, 0, 0), _max(0, 0, 0) {}

    AABB::~AABB() {}

    bool AABB::calculateEntryPoint(const Ray& ray) const
    {
        double tx1  = (_min.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
        double tx2 = (_max.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
        double tminX = std::min(tx1, tx2);
        double tmaxX = std::max(tx1, tx2);

        double ty1  = (_min.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
        double ty2 = (_max.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
        double tminY = std::min(ty1, ty2);
        double tmaxY = std::max(ty1, ty2);

        double tz1  = (_min.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
        double tz2 = (_max.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
        double tminZ = std::min(tz1, tz2);
        double tmaxZ = std::max(tz1, tz2);

        double entryPoint = std::max(std::max(tminX, tminY), tminZ);
        double exitPoint  = std::min(std::min(tmaxX, tmaxY), tmaxZ);
        if (entryPoint > exitPoint || exitPoint < 0)
            return false;
        return true;
    }

    HitInfo AABB::getHitInfo(const Ray& ray, const Material &material) const
    {
        HitInfo hitInfo;
        double tx1  = (_min.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
        double tx2 = (_max.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
        double tminX = std::min(tx1, tx2);
        double tmaxX = std::max(tx1, tx2);

        double ty1  = (_min.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
        double ty2 = (_max.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
        double tminY = std::min(ty1, ty2);
        double tmaxY = std::max(ty1, ty2);

        double tz1  = (_min.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
        double tz2 = (_max.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
        double tminZ = std::min(tz1, tz2);
        double tmaxZ = std::max(tz1, tz2);

        double entryPoint = std::max(std::max(tminX, tminY), tminZ);
        double exitPoint  = std::min(std::min(tmaxX, tmaxY), tmaxZ);

        if (entryPoint > exitPoint || exitPoint < 0)
            return hitInfo;
        Math::Vector3D normal(1.0, 0.0, 0.0);

        if (entryPoint == tminX) {
            if ((ray.getDirection().getX() < 0))
                normal.setX(1.0);
            else
                normal.setX(-1.0);
        } else if (entryPoint == tminY) {
            if ((ray.getDirection().getY() < 0))
                normal.setY(1.0);
            else
                normal.setY(-1.0);
        } else if (entryPoint == tminZ) {
            if ((ray.getDirection().getZ() < 0))
                normal.setZ(1.0);
            else
                normal.setZ(-1.0);
        }
        hitInfo.normal = normal;
        hitInfo.hit = true;
        hitInfo.distance = entryPoint;
        hitInfo.originDirection = ray.getDirection() * -1;
        hitInfo.point = ray.getOrigin() + ray.getDirection() * entryPoint;
        double u = 0.0, v = 0.0;
        std::string faceId;

        if (entryPoint == tminX) {
            if (ray.getDirection().getX() < 0)
                faceId = "-X";
            else
                faceId = "+X";
            u = (hitInfo.point.getZ() - _min.getZ()) / (_max.getZ() - _min.getZ());
            v = (hitInfo.point.getY() - _min.getY()) / (_max.getY() - _min.getY());
        } else if (entryPoint == tminY) {
            if (ray.getDirection().getY() < 0)
                faceId = "-Y";
            else
                faceId = "+Y";
            u = (hitInfo.point.getX() - _min.getX()) / (_max.getX() - _min.getX());
            v = (hitInfo.point.getZ() - _min.getZ()) / (_max.getZ() - _min.getZ());
        } else if (entryPoint == tminZ) {
            if (ray.getDirection().getZ() < 0)
                faceId = "-Z";
            else
                faceId = "+Z";
            u = (hitInfo.point.getX() - _min.getX()) / (_max.getX() - _min.getX());
            v = (hitInfo.point.getY() - _min.getY()) / (_max.getY() - _min.getY());
        }
        material.getColor(u, v, faceId);
        // const auto& texIt = material.textureList.find(faceId);
        // if (texIt != material.textureList.end()) {
        //     hitInfo.color = texIt->second.getTexture().getColor(u, v);
        // } else if (material.texture.hasTexture()) {
        //     hitInfo.color = material.texture.getColor(u, v);
        // } else {
        //     hitInfo.color = material._color;
        // }


        return hitInfo;
    }
}