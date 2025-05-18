/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Materials
*/

#include "Materials.hpp"

namespace RayTracer {

    Material::Material(std::string name, ArgumentMap params)
    {
        _name = name;
        if (params["color"].exists<Color>())
            _color = params["color"].as<Color>();
        if (params["opacity"].exists<double>())
            opacity = params["opacity"].as<double>();
        if (params["texture"].exists<std::string>())
            texture = Texture(params["texture"].as<std::string>());
        if (params["textureScale"].exists<double>())
            textureScale = 1 / params["textureScale"].as<double>();
        if (params["reflexion"].exists<double>())
            reflectivity = params["reflexion"].as<double>();
        if (params["shininess"].exists<double>())
            shininess = params["shininess"].as<double>();
        if (params["textureList"].exists<ArgumentMap>()) {
            ArgumentMap textures = params["textureList"].as<ArgumentMap>();
            for (auto &i : textures) {
                Texture newTexture = Texture(std::any_cast<std::string>(i.second));
                Material newMaterial;
                newMaterial.texture = newTexture;
                newMaterial._color = _color;
                newMaterial.opacity = opacity;
                newMaterial.reflectivity = reflectivity;
                newMaterial.textureScale = textureScale;
                textureList[i.first] = newMaterial;
            }
        }
    }
}
