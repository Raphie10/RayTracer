/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** test_vector.cpp
*/

#include <criterion/criterion.h>
#include "Vector.hpp"

Test(vector_tests, create_vector) {
    Math::Vector3D vec(1.0, 2.0, 3.0);

    cr_assert_float_eq(vec.getX(), 1.0, 1e-6, "X coordinate should be 1.0");
    cr_assert_float_eq(vec.getY(), 2.0, 1e-6, "Y coordinate should be 2.0");
    cr_assert_float_eq(vec.getZ(), 3.0, 1e-6, "Z coordinate should be 3.0");
}

Test(vector_tests, vector_length) {
    Math::Vector3D vec(1.0, 2.0, 3.0);

    double expected_length = sqrt(14.0);
    cr_assert_float_eq(vec.length(), expected_length, 1e-6, "Vector length should be √14");
}

Test(vector_tests, vector_normalize) {
    Math::Vector3D vec(1.0, 2.0, 3.0);
    Math::Vector3D normalized = vec.normalize();

    double length = vec.length();
    cr_assert_float_eq(normalized.getX(), vec.getX() / length, 1e-6, "X should be divided by length");
    cr_assert_float_eq(normalized.getY(), vec.getY() / length, 1e-6, "Y should be divided by length");
    cr_assert_float_eq(normalized.getZ(), vec.getZ() / length, 1e-6, "Z should be divided by length");
    cr_assert_float_eq(normalized.length(), 1.0, 1e-6, "Normalized vector length should be 1.0");
}

Test(vector_tests, dot_product) {
    Math::Vector3D vec1(1.0, 2.0, 3.0);
    Math::Vector3D vec2(4.0, 5.0, 6.0);

    cr_assert_float_eq(vec1.dot(vec2), 32.0, 1e-6, "Dot product should be 32");
}

Test(vector_tests, vector_addition) {
    Math::Vector3D vec1(1.0, 2.0, 3.0);
    Math::Vector3D vec2(4.0, 5.0, 6.0);
    Math::Vector3D result = vec1 + vec2;

    cr_assert_float_eq(result.getX(), 5.0, 1e-6, "X should be 5.0");
    cr_assert_float_eq(result.getY(), 7.0, 1e-6, "Y should be 7.0");
    cr_assert_float_eq(result.getZ(), 9.0, 1e-6, "Z should be 9.0");
}

Test(vector_tests, vector_subtraction) {
    Math::Vector3D vec1(4.0, 5.0, 6.0);
    Math::Vector3D vec2(1.0, 2.0, 3.0);
    Math::Vector3D result = vec1 - vec2;

    cr_assert_float_eq(result.getX(), 3.0, 1e-6, "X should be 3.0");
    cr_assert_float_eq(result.getY(), 3.0, 1e-6, "Y should be 3.0");
    cr_assert_float_eq(result.getZ(), 3.0, 1e-6, "Z should be 3.0");
}

Test(vector_tests, cross_product) {
    Math::Vector3D vec1(1.0, 0.0, 0.0);
    Math::Vector3D vec2(0.0, 1.0, 0.0);
    Math::Vector3D result = vec1.cross(vec2);

    cr_assert_float_eq(result.getX(), 0.0, 1e-6, "X should be 0.0");
    cr_assert_float_eq(result.getY(), 0.0, 1e-6, "Y should be 0.0");
    cr_assert_float_eq(result.getZ(), 1.0, 1e-6, "Z should be 1.0");
}
