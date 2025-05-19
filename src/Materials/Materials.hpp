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
            Texture getTexture() const { return texture; }
            bool isTransparent() const { return opacity < 1.0f; }
            Color getColor() const { return _color; }
            std::unordered_map<std::string, Material> &getMaterialList() { return textureList; }
            double getOpacity() const { return opacity; }
            double getReflectivity() const { return reflectivity; }
            double getTextureScale() const { return textureScale; }
            double getShininess() const { return shininess; }
            void setColor(const Color &color) { _color = color; }
            void setOpacity(double opacity) { this->opacity = opacity; }
            void setReflectivity(double reflectivity) { this->reflectivity = reflectivity; }
            void setTextureScale(double scale) { this->textureScale = scale; }
            void setTexture(const Texture &texture) { this->texture = texture; }
            void setShininess(double shininess) { this->shininess = shininess; }
            void setName(const std::string &name) { _name = name; }
        private:
            std::string _name = "material";
            Color _color = Color(1, 1, 1);
            double opacity = 1.0;
            double reflectivity = 0.0;
            Texture texture;
            std::unordered_map<std::string, Material> textureList;
            double textureScale;
            double shininess = 10.0;
    };
}