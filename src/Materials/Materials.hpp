/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Materials
*/

#pragma once

#include "Color.hpp"
#include <iostream>
#include <ArgumentMap.hpp>
#include "Texture.hpp"

namespace RayTracer {
    class Material{
        public:
            Material() = default;
            Material(std::string name, ArgumentMap params);
            Color getColor(double u, double v) const {
                if (texture.hasTexture()) {
                    return texture.getColor(u, v);
                }
                return _color;
            }
            Color getColor(double u, double v, std::string textureName) const {
                    const auto &tex = textureList.find(textureName);
                if (tex != textureList.end()) {
                    return tex->second.getTexture().getColor(u, v);
                } else if (texture.hasTexture()) {
                    return texture.getColor(u, v);
                } else {
                    return _color;
                }
            }
            std::string _name = "material";
            Color _color = Color(1, 1, 1);
            double opacity = 1.0;
            double reflectivity = 0.0;
            Texture texture;
            std::unordered_map<std::string, Material> textureList;
            double textureScale;
            double shininess = 10.0;
            Texture getTexture() const { return texture; }
            bool isTransparent() const { return opacity < 1.0f; }
    };
}