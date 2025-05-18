/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** test_point.cpp
*/

#include <criterion/criterion.h>
#include "Points.hpp"
#include "Vector.hpp"

Test(point_tests, create_point) {
    Math::Point3D point(1.0, 2.0, 3.0);

    cr_assert_float_eq(point.getX(), 1.0, 1e-6, "X coordinate should be 1.0");
    cr_assert_float_eq(point.getY(), 2.0, 1e-6, "Y coordinate should be 2.0");
    cr_assert_float_eq(point.getZ(), 3.0, 1e-6, "Z coordinate should be 3.0");
}

Test(point_tests, point_plus_vector) {
    Math::Point3D point(1.0, 2.0, 3.0);
    Math::Vector3D vec(4.0, 5.0, 6.0);

    Math::Point3D result = point + vec;

    cr_assert_float_eq(result.getX(), 5.0, 1e-6, "X should be 5.0");
    cr_assert_float_eq(result.getY(), 7.0, 1e-6, "Y should be 7.0");
    cr_assert_float_eq(result.getZ(), 9.0, 1e-6, "Z should be 9.0");
}

Test(point_tests, point_plus_point) {
    Math::Point3D point1(1.0, 2.0, 3.0);
    Math::Point3D point2(4.0, 5.0, 6.0);

    Math::Point3D result = point1 + point2;

    cr_assert_float_eq(result.getX(), 5.0, 1e-6, "X should be 5.0");
    cr_assert_float_eq(result.getY(), 7.0, 1e-6, "Y should be 7.0");
    cr_assert_float_eq(result.getZ(), 9.0, 1e-6, "Z should be 9.0");
}

Test(point_tests, point_minus_point) {
    Math::Point3D point1(4.0, 5.0, 6.0);
    Math::Point3D point2(1.0, 2.0, 3.0);

    Math::Vector3D result = point1 - point2;

    cr_assert_float_eq(result.getX(), 3.0, 1e-6, "X should be 3.0");
    cr_assert_float_eq(result.getY(), 3.0, 1e-6, "Y should be 3.0");
    cr_assert_float_eq(result.getZ(), 3.0, 1e-6, "Z should be 3.0");
}

Test(point_tests, point_times_scalar) {
    Math::Point3D point(1.0, 2.0, 3.0);

    Math::Point3D result = point * 2;

    cr_assert_float_eq(result.getX(), 2.0, 1e-6, "X should be 2.0");
    cr_assert_float_eq(result.getY(), 4.0, 1e-6, "Y should be 4.0");
    cr_assert_float_eq(result.getZ(), 6.0, 1e-6, "Z should be 6.0");
}

Test(point_tests, point_divide_scalar) {
    Math::Point3D point(2.0, 4.0, 6.0);

    Math::Point3D result = point / 2;

    cr_assert_float_eq(result.getX(), 1.0, 1e-6, "X should be 1.0");
    cr_assert_float_eq(result.getY(), 2.0, 1e-6, "Y should be 2.0");
    cr_assert_float_eq(result.getZ(), 3.0, 1e-6, "Z should be 3.0");
}
