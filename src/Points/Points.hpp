/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Points.hpp
*/
#pragma once

#include <cmath>
#include "../Vectors/Vector.hpp"


namespace Math {

    class Point3D {
        public:

            Point3D() : X(0), Y(0), Z(0) {}
            Point3D(double x, double y, double z) : X(x), Y(y), Z(z) {}

            Point3D operator+(const Vector3D& vec) const;
            Point3D operator+(const Point3D vec) const;
            Point3D& operator+=(const Vector3D& vec);
            Vector3D operator-(const Point3D& other) const;
            Point3D operator*(const int val) const;
            Point3D operator/(const int val) const;

            double getX() const { return X; }
            double getY() const { return Y; }
            double getZ() const { return Z; }

            void setX(double x) { X = x; }
            void setY(double y) { Y = y; }
            void setZ(double z) { Z = z; }

        private:
            double X;
            double Y;
            double Z;
    };
}