/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** RayTracer.hpp
*/

#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "../Camera/Camera.hpp"
#include "../Rectangle3D/Screen.hpp"
#include "IPrimitive.hpp"
#include "ILights.hpp"
#include "../Parsing/Parsing_cfg.hpp"
#include "../DynamicLibrary/DynamicLibrary.hpp"
#include "Color.hpp"
#include "../Materials/Materials.hpp"
#include "Scene.hpp"
#include "LibraryManager.hpp"

namespace RayTracer {
class RenderWorkerPool;
    class RayCaster {
        private:
            Screen _screen;
            int maxDepth;
            int samplesPerPixel;
            int blur;
            std::unique_ptr<RenderWorkerPool> _renderWorkerPool;
            std::mutex screenMutex;
            std::atomic<bool> renderingActive;
            std::atomic<int> currentLine;
            class RayTracerError : public std::exception {
                public:
                    RayTracerError(const std::string message) : msg(message) {}
                    const char* what() const noexcept override {
                        return msg.c_str();
                    }
                private:
                    std::string msg;
            };

        public:
            RayCaster(ArgumentMap resolution, Parsing_cfg &parsedData);
            ~RayCaster();

            Color renderPixel(int x, int y, std::shared_ptr<Scene> &scene);
            void renderLine(int y, std::shared_ptr<Scene> scene);

            void start_rendering(std::shared_ptr<Scene> scene);
            void stopRendering();
            void restartRendering(std::shared_ptr<Scene> &scene);
            bool isRaytracingDone() const { return !( getCurrentLine() < _screen.getHeight()) || !renderingActive.load(); }
            bool isRenderingActive() const { return renderingActive.load(); }
            int getCurrentLine() const { return currentLine; }
            void setCurrentLine(int y) {currentLine = y;};
            void renderLoop(std::shared_ptr<Scene> scene);

            void saveImage(const std::string& filename) const;
            Screen& getScreen() { return _screen; }
    };

}

