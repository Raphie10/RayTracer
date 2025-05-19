/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** test_color.cpp
*/

#include <criterion/criterion.h>
#include "Color.hpp"

Test(color_tests, create_color) {
    RayTracer::Color color(0.1, 0.2, 0.3);

    cr_assert_float_eq(color.getR(), 0.1, 1e-6, "Red should be 0.1");
    cr_assert_float_eq(color.getG(), 0.2, 1e-6, "Green should be 0.2");
    cr_assert_float_eq(color.getB(), 0.3, 1e-6, "Blue should be 0.3");
    cr_assert_float_eq(color.getA(), 1.0, 1e-6, "Alpha should be 1.0 by default");
}

Test(color_tests, color_addition) {
    RayTracer::Color color1(0.1, 0.2, 0.3);
    RayTracer::Color color2(0.2, 0.3, 0.4);

    RayTracer::Color result = color1 + color2;

    cr_assert_float_eq(result.getR(), 0.3, 1e-6, "Red should be 0.3");
    cr_assert_float_eq(result.getG(), 0.5, 1e-6, "Green should be 0.5");
    cr_assert_float_eq(result.getB(), 0.7, 1e-6, "Blue should be 0.7");
}

Test(color_tests, color_multiplication) {
    RayTracer::Color color1(0.5, 0.6, 0.7);
    RayTracer::Color color2(0.2, 0.3, 0.4);

    RayTracer::Color result = color1 * color2;

    cr_assert_float_eq(result.getR(), 0.1, 1e-6, "Red should be 0.1");
    cr_assert_float_eq(result.getG(), 0.18, 1e-6, "Green should be 0.18");
    cr_assert_float_eq(result.getB(), 0.28, 1e-6, "Blue should be 0.28");
}

Test(color_tests, color_scalar_multiplication) {
    RayTracer::Color color(0.1, 0.2, 0.3);

    RayTracer::Color result = color * 2.0;

    cr_assert_float_eq(result.getR(), 0.2, 1e-6, "Red should be 0.2");
    cr_assert_float_eq(result.getG(), 0.4, 1e-6, "Green should be 0.4");
    cr_assert_float_eq(result.getB(), 0.6, 1e-6, "Blue should be 0.6");
}

Test(color_tests, color_scalar_division) {
    RayTracer::Color color(0.2, 0.4, 0.6);

    RayTracer::Color result = color / 2.0;

    cr_assert_float_eq(result.getR(), 0.1, 1e-6, "Red should be 0.1");
    cr_assert_float_eq(result.getG(), 0.2, 1e-6, "Green should be 0.2");
    cr_assert_float_eq(result.getB(), 0.3, 1e-6, "Blue should be 0.3");
}
