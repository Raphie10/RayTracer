/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Texture
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Color.hpp"
#include <iostream>
#include <memory>
#include "Points.hpp"

namespace RayTracer {

    class Texture {
        public:
            Texture() = default;
            Texture(std::string textureName) : textureName(textureName)
            {
                if (textureName == "checker") {
                    textureImage->create(1, 1, sf::Color::Red);
                    return;
                }
                textureImage->loadFromFile(textureName);
                if (textureImage->getSize().x == 0 || textureImage->getSize().y == 0) {
                    std::cerr << "Error: Texture file not found or empty: " << textureName << std::endl;
                }
            }
            ~Texture();
            Color getColor(double u, double v) const;
            void addTextureCoord(Math::Point3D point)
            {
                textureCoords.push_back(point);
            }
            bool hasTexture() const {
                return (textureImage->getSize().x > 0 && textureImage->getSize().y > 0) || textureName == "checker";
            }
            const std::vector<Math::Point3D> &getTextureCoords() const
            {
                return textureCoords;
            }
        private:
            std::vector<Math::Point3D> textureCoords;
            std::string textureName;
            std::shared_ptr<sf::Image> textureImage = std::make_shared<sf::Image>();
    };
}