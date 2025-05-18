/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** RenderWorker
*/

#include "RenderWorkerPool.hpp"

namespace RayTracer {
    RenderWorkerPool::RenderWorkerPool(RayCaster &raycaster, Scene &scene) :
        _raycaster(raycaster), _scene(scene)
    {
    }

    RenderWorkerPool::~RenderWorkerPool()
    {
    }

    void RenderWorkerPool::startWorkers()
    {
        lineToRender.store(0);

        int numThreads = std::thread::hardware_concurrency() - 2;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&RenderWorkerPool::worker,  this);
        }
        for (auto& t : threads) {
            t.join();
        }
    }

    void RenderWorkerPool::worker()
    {
        while (_raycaster.isRenderingActive()) {
            int line = lineToRender.fetch_add(1);
            if (line >= _raycaster.getScreen().getHeight()) {
                break;
            }
            _raycaster.renderLine(line, _scene);
        }
    }

    void RenderWorkerPool::stopWorkers()
    {
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads.clear();
    }
}