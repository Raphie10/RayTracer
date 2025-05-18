/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Texture
*/

#include "Texture.hpp"
#include <cmath>

namespace RayTracer {

    Texture::~Texture()
    {
    }

    Color Texture::getColor(double u, double v) const
    {
        if (textureName == "checker") {
            u = u - std::floor(u);
            v = v - std::floor(v);

            int i = static_cast<int>(std::floor(u * 2)) % 2;
            int j = static_cast<int>(std::floor(v * 2)) % 2;
            if ((i + j) % 2 == 0) {
                return Color(1.0, 1.0, 1.0);
            } else {
                return Color(0.0, 0.0, 0.0);
            }
        }
        u = u - std::floor(u);
        v = v - std::floor(v);

        sf::Vector2u size = textureImage->getSize();

        if (size.x <= 0)
            return Color (0, 0, 0);

        unsigned int x = static_cast<unsigned>(u * size.x);
        unsigned int y = static_cast<unsigned>((1 - v) * size.y);
        sf::Color pixel = textureImage->getPixel(x % size.x, y % size.y);
        return Color (pixel.r / 255.f, pixel.g / 255.f, pixel.b / 255.f);
    }
}