/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Additional unit tests for RayTracer project
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <math.h>
#include <limits>
#include "../Ray/Ray.hpp"
#include "Vector.hpp"
#include "Points.hpp"
#include "../BVHBuilder/AABB.hpp"
#include "../BVHBuilder/BinaryTree.hpp"
#include "ArgumentMap.hpp"
#include "../Rectangle3D/Screen.hpp"
#include "../Camera/Camera.hpp"
#include "../Materials/Materials.hpp"

// Tests pour Ray
Test(ray, constructor)
{
    Math::Point3D origin(1.0, 2.0, 3.0);
    Math::Vector3D direction(0.0, 1.0, 0.0);
    RayTracer::Ray ray(origin, direction);

    cr_assert_float_eq(ray.getOrigin().getX(), 1.0, 1e-6, "Ray origin x coordinate should be 1.0");
    cr_assert_float_eq(ray.getOrigin().getY(), 2.0, 1e-6, "Ray origin y coordinate should be 2.0");
    cr_assert_float_eq(ray.getOrigin().getZ(), 3.0, 1e-6, "Ray origin z coordinate should be 3.0");

    cr_assert_float_eq(ray.getDirection().getX(), 0.0, 1e-6, "Ray direction x component should be 0.0");
    cr_assert_float_eq(ray.getDirection().getY(), 1.0, 1e-6, "Ray direction y component should be 1.0");
    cr_assert_float_eq(ray.getDirection().getZ(), 0.0, 1e-6, "Ray direction z component should be 0.0");
}

Test(ray, default_constructor)
{
    RayTracer::Ray ray;

    cr_assert_float_eq(ray.getOrigin().getX(), 0.0, 1e-6, "Default ray origin x should be 0.0");
    cr_assert_float_eq(ray.getOrigin().getY(), 0.0, 1e-6, "Default ray origin y should be 0.0");
    cr_assert_float_eq(ray.getOrigin().getZ(), 0.0, 1e-6, "Default ray origin z should be 0.0");
}

// Tests pour AABB
Test(aabb, constructor)
{
    Math::Point3D min(1.0, 2.0, 3.0);
    Math::Point3D max(4.0, 5.0, 6.0);

    RayTracer::AABB box(min, max);

    cr_assert_float_eq(box.getMin().getX(), 1.0, 1e-6, "AABB min point x should be 1.0");
    cr_assert_float_eq(box.getMin().getY(), 2.0, 1e-6, "AABB min point y should be 2.0");
    cr_assert_float_eq(box.getMin().getZ(), 3.0, 1e-6, "AABB min point z should be 3.0");

    cr_assert_float_eq(box.getMax().getX(), 4.0, 1e-6, "AABB max point x should be 4.0");
    cr_assert_float_eq(box.getMax().getY(), 5.0, 1e-6, "AABB max point y should be 5.0");
    cr_assert_float_eq(box.getMax().getZ(), 6.0, 1e-6, "AABB max point z should be 6.0");
}

Test(aabb, intersect)
{
    Math::Point3D min(-1.0, -1.0, -1.0);
    Math::Point3D max(1.0, 1.0, 1.0);
    RayTracer::AABB box(min, max);
    RayTracer::Material test;

    RayTracer::Ray ray1(Math::Point3D(0.0, 0.0, -2.0), Math::Vector3D(0.0, 0.0, 1.0));
    RayTracer::HitInfo hitInfo1 = box.getHitInfo(ray1, test);
    cr_assert(hitInfo1.hit, "Ray should intersect the AABB");

    RayTracer::Ray ray2(Math::Point3D(2.0, 2.0, 2.0), Math::Vector3D(1.0, 1.0, 1.0));
    RayTracer::HitInfo hitInfo2 = box.getHitInfo(ray2, test);
    cr_assert_not(hitInfo2.hit, "Ray should not intersect the AABB");
}

Test(argument_map, constructor)
{
    RayTracer::ArgumentMap args;

    cr_assert_eq(args.getMap().size(), 0, "New ArgumentMap should be empty");
}

Test(argument_map, add_and_get)
{
    RayTracer::ArgumentMap args;
    args["name"] = std::string("raytracer");
    args["width"] = 800;

    cr_assert_str_eq(args["name"].as<std::string>().c_str(), "raytracer", "String value should match what was added");
    cr_assert_eq(args["width"].as<int>(), 800, "Integer value should match what was added");
}

// Tests pour Screen
Test(screen, constructor)
{
    int width = 800;
    int height = 600;
    RayTracer::Screen screen(width, height);

    cr_assert_eq(screen.getWidth(), width, "Screen width should be 800");
    cr_assert_eq(screen.getHeight(), height, "Screen height should be 600");
}

Test(screen, set_get_pixel)
{
    RayTracer::Screen screen(10, 10);
    RayTracer::Color color(0.5, 0.6, 0.7);

    screen.setPixel(5, 5, color);
    RayTracer::Color retrieved = screen.getPixel(5, 5);

    cr_assert_float_eq(retrieved.R, 0.5, 1e-6, "Retrieved color R should be 0.5");
    cr_assert_float_eq(retrieved.G, 0.6, 1e-6, "Retrieved color G should be 0.6");
    cr_assert_float_eq(retrieved.B, 0.7, 1e-6, "Retrieved color B should be 0.7");

    RayTracer::Color color2(0.8, 0.9, 1.0);
    screen.setPixel(0, color2);
    RayTracer::Color retrieved2 = screen.getPixel(0, 0);

    cr_assert_float_eq(retrieved2.R, 0.8, 1e-6, "Retrieved color R should be 0.8");
    cr_assert_float_eq(retrieved2.G, 0.9, 1e-6, "Retrieved color G should be 0.9");
    cr_assert_float_eq(retrieved2.B, 1.0, 1e-6, "Retrieved color B should be 1.0");
}

// Tests pour Camera
Test(camera, constructor)
{
    Math::Point3D position(1.0, 2.0, 3.0);
    Math::Vector3D direction(0.0, 0.0, -1.0);
    Math::Vector3D up(0.0, 1.0, 0.0);
    double fov = 60.0;
    double aspectRatio = 1.5;
    RayTracer::Camera camera(position, direction, up, fov, aspectRatio);

    cr_assert_float_eq(camera.getPosition().getX(), 1.0, 1e-6, "Camera position x should be 1.0");
    cr_assert_float_eq(camera.getPosition().getY(), 2.0, 1e-6, "Camera position y should be 2.0");
    cr_assert_float_eq(camera.getPosition().getZ(), 3.0, 1e-6, "Camera position z should be 3.0");

    cr_assert_float_eq(camera.getDirection().getX(), 0.0, 1e-6, "Camera direction x should be 0.0");
    cr_assert_float_eq(camera.getDirection().getY(), 0.0, 1e-6, "Camera direction y should be 0.0");
    cr_assert_float_eq(camera.getDirection().getZ(), -1.0, 1e-6, "Camera direction z should be -1.0");
}

Test(camera, generate_ray)
{
    RayTracer::Camera camera(Math::Point3D(0, 0, 0), Math::Vector3D(0, 0, -1), Math::Vector3D(0, 1, 0), 90.0, 1.0);
    RayTracer::Ray ray = camera.generate_ray(0.5, 0.5);

    cr_assert_float_eq(ray.getOrigin().getX(), 0.0, 1e-6, "Generated ray origin X should be 0.0");
    cr_assert_float_eq(ray.getOrigin().getY(), 0.0, 1e-6, "Generated ray origin Y should be 0.0");
    cr_assert_float_eq(ray.getOrigin().getZ(), 0.0, 1e-6, "Generated ray origin Z should be 0.0");
}
