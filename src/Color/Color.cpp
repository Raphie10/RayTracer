/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Color
*/

#include "Color.hpp"

namespace RayTracer {

    Color::~Color() {}

    Color Color::operator+(const Color& other) const
    {
        return (Color(R + other.R, G + other.G, B + other.B));
    }

    Color& Color::operator+=(const Color& other)
    {
        R += other.R;
        G += other.G;
        B += other.B;
        return *this;
    }

    Color Color::operator*(const Color& other) const
    {
        return (Color(R * other.R, G * other.G, B * other.B));
    }

    Color& Color::operator*=(const Color& other)
    {
        R *= other.R;
        G *= other.G;
        B *= other.B;
        return *this;
    }

    Color Color::operator*(double scalar) const
    {
        return Color(R * scalar, G * scalar, B * scalar);
    }

    Color& Color::operator*=(double scalar)
    {
        this->R *= scalar;
        this->G *= scalar;
        this->B *= scalar;
        return *this;
    }

    Color Color::operator/(double scalar) const
    {
        return Color(R / scalar, G / scalar, B / scalar);
    }

    Color& Color::operator/=(double scalar)
    {
        this->R /= scalar;
        this->G /= scalar;
        this->B /= scalar;
        return *this;
    }

}