/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Color
*/

#pragma once

namespace RayTracer {

    class Color {
        public:
            Color(int r, int g, int b) : R(r / 255), G(g / 255), B(b / 255), A(1.0){};
            Color(int r, int g, int b, int a) : R(r / 255), G(g / 255), B(b / 255), A(a / 255){};
            Color(double r, double g, double b) : R(r), G(g), B(b), A(1.0){}
            Color(double r, double g, double b, double a) : R(r), G(g), B(b), A(a){}
            Color() : R(0), G(0), B(0), A(0){}
            ~Color();

            Color operator+(const Color& other) const;
            Color& operator+=(const Color& other);
            Color operator*(const Color& other) const;
            Color& operator*=(const Color& other);
            Color operator*(double scalar) const;
            Color& operator*=(double scalar);
            Color operator/(double scalar) const;
            Color& operator/=(double scalar);
            double getR() const { return R; }
            double getG() const { return G; }
            double getB() const { return B; }
            double getA() const { return A; }
        private:
            double R;
            double G;
            double B;
            double A;
    };
}