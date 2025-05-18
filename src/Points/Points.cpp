/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Points.cpp
*/

#include "Points.hpp"

namespace Math {

    Point3D Point3D::operator+(const Vector3D& vec) const
    {
        return Point3D(X + vec.getX(), Y + vec.getY(), Z + vec.getZ());
    }

    Point3D Point3D::operator+(const Point3D vec) const
    {
        return Point3D(X + vec.getX(), Y + vec.getY(), Z + vec.getZ());
    }

    Point3D& Point3D::operator+=(const Vector3D& vec)
    {
        X += vec.getX();
        Y += vec.getY();
        Z += vec.getZ();
        return *this;
    }

    Vector3D Point3D::operator-(const Point3D& other) const
    {
        return Vector3D(X - other.getX(), Y - other.getY(), Z - other.getZ());
    }

    Point3D Point3D::operator*(const int val) const
    {
        return Point3D(X * val, Y * val, Z * val);
    }

    Point3D Point3D::operator/(const int val) const
    {
        return Point3D(X / val, Y / val, Z / val);
    }
}