/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** RenderWorker
*/

#pragma once

#include <thread>
#include <vector>
#include "RayTracer.hpp"

namespace RayTracer {

    class RenderWorkerPool {
        public:
            RenderWorkerPool(RayCaster &raycaster, Scene &scene);
            ~RenderWorkerPool();
            void startWorkers();
            void worker();
            void stopWorkers();
        private:
            RayCaster &_raycaster;
            Scene &_scene;
            std::vector<std::thread> threads;
            std::atomic<int> lineToRender = 0;
    };
}