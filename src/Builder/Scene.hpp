/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Scene
*/

#pragma once

#include <vector>
#include <memory>
#include "../Camera/Camera.hpp"
#include "Parsing_cfg.hpp"
#include "../DynamicLibrary/DynamicLibrary.hpp"
#include "LibraryManager.hpp"
#include "../BVHBuilder/AABB.hpp"
#include "../BVHBuilder/BinaryTree.hpp"

namespace RayTracer {

    class Scene {
        public:
            Scene(Parsing_cfg &parsedData, LibraryManager &libraryHandles) :
                _camera(parsedData.getCamInfo())
            {
                buildScene(parsedData, libraryHandles);
            };
            ~Scene();
            

            Camera &getCamera() { return _camera; }
            const Math::Vector3D getCameraRight() const {
                return _camera.getDirection().cross(_camera.getUp()).normalize();
            }
            const Math::Vector3D &getCameraDirection() const {
                return _camera.getDirection();
            }

            const std::vector<std::unique_ptr<IPrimitive>> &getPrimitives() const { return _primitives; }
            const std::unique_ptr<Node> &getTree() const { return tree; }
            const std::vector<std::unique_ptr<ILights>> &getLights() const { return _lights; }
            void buildScene(Parsing_cfg &parsedData, LibraryManager &libraryHandles);
            void clear() {
                _primitives.clear();
                _lights.clear();
            }
            void addPrimitive(std::unique_ptr<IPrimitive> primitive) {
                _primitives.push_back(std::move(primitive));
            }
            void addLight(std::unique_ptr<ILights> light) {
                _lights.push_back(std::move(light));
            }
            void moveCamera(const Math::Vector3D &offset) {
                _camera.move(offset);
            }
            void changeCameraOrientation(const Math::Vector3D &direction) {
                _camera.rotate(direction);
            }
            Color getAmbientLight() const { return ambientLight; }
            Color getBackgroundColor() const { return backgroundColor; }
        private:
            Color backgroundColor;
            Color ambientLight;
            Camera _camera;
            std::vector<std::unique_ptr<IPrimitive>> _primitives;
            std::unique_ptr<Node> tree;
            std::vector<std::unique_ptr<ILights>> _lights;
            std::map<std::string, Material> materials;
    };
}