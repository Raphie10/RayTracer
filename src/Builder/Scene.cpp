/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Scene
*/

#include "Scene.hpp"
#include "IPrimitive.hpp"
#include "ILights.hpp"

namespace RayTracer {

    Scene::~Scene()
    {
        _primitives.clear();
        _lights.clear();
    }

    void Scene::buildScene(const Parsing_cfg &parsedData, LibraryManager &libraryHandles)
    {
        for (const auto &i : parsedData.getMaterialsInfo()) {
            materials[i.first] = Material(i.first, i.second);
        }
        const std::unordered_map<std::string, std::vector<ArgumentMap>> &primitivesInfo = parsedData.getPrimitiveInfo();
        for (const auto &primitive : primitivesInfo) {
            std::unique_ptr<DynamicLibrary> &currentPrimitive = libraryHandles.getCurrentLibrary(primitive.first, "getPrimitiveName");

            auto constructor = currentPrimitive->getConstructor<IPrimitive, ArgumentMap>("createPrimitive");

            for (auto params : primitive.second) {
                if (params["material"].exists<std::string>()) {
                    std::string materialName = params["material"].as<std::string>();
                    params["material"] = std::ref(this->materials[materialName]);
                }
                addPrimitive(constructor(params));
            }
        }

        const std::unordered_map<std::string, std::vector<ArgumentMap>> &lightsInfo = parsedData.getLightsInfo();
            for (const auto &light : lightsInfo) {
                std::unique_ptr<DynamicLibrary> &currentLight = libraryHandles.getCurrentLightLibrary(light.first, "getLightName");
                auto constructor = currentLight->getConstructor<ILights, ArgumentMap>("createLight");
                for (auto params : light.second) {
                    addLight(constructor(params));
                }
            }
    }
}
