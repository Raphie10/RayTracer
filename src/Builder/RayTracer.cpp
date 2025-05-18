/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** RayTracer.cpp
*/

#include "RayTracer.hpp"
#include <iostream>
#include <thread>
#include "../DynamicLibrary/DynamicLibrary.hpp"
#include <filesystem>
#include "../Parsing/ArgumentMap.hpp"
#include "../Materials/Materials.hpp"
#include "../Ray/Ray.hpp"
#include "RenderWorkerPool.hpp"

namespace RayTracer {

    RayCaster::RayCaster(ArgumentMap resolution) :
        _screen(resolution),
        maxDepth(5), samplesPerPixel(1), renderingActive(false), currentLine(0)
    {
    }

    RayCaster::~RayCaster()
    {
        stopRendering();
    }

    void RayCaster::start_rendering(Scene &scene)
    {
        if (renderingActive.load()) {
            return;
        }
        currentLine.store(0);
        renderingActive.store(true);
        std::thread renderThread(&RayCaster::renderLoop, this, std::ref(scene));
        renderThread.detach();
    }

    void RayCaster::restartRendering(Scene &scene)
    {
        stopRendering();
        _screen.resetCompletedLines();
        start_rendering(scene);
    }

    void RayCaster::stopRendering()
    {
        if (!renderingActive.load())
            return;
        renderingActive.store(false);
        std::string outputFile = "output.ppm";
        saveImage(outputFile);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    Color RayCaster::renderPixel(int x, int y, Scene &scene)
    {
        Color color(0, 0, 0);
        double u, v;
        _screen.getUV(x, y, u, v);
        for (int s = 0; s < samplesPerPixel; s++) {
            if (samplesPerPixel > 1) {
                double du = 1.0 / _screen.getWidth();
                double dv = 1.0 / _screen.getHeight();
                u += du * (rand() / static_cast<double>(RAND_MAX) - 0.5);
                v += dv * (rand() / static_cast<double>(RAND_MAX) - 0.5);
            }
            Ray ray = scene.getCamera().generate_ray(u, v);
            color += ray.trace_ray(scene, maxDepth);
        }
        if (samplesPerPixel > 1) {
            color = color / static_cast<double>(samplesPerPixel);
        }
        color = Color(
            sqrt(color.R),
            sqrt(color.G),
            sqrt(color.B)
        );
        return color;
    }

    void RayCaster::renderLine(int y, Scene &scene)
    {
        int width = _screen.getWidth();
        std::vector<Color> lineColors(width);
        for (int x = 0; x < width && renderingActive.load(); x++) {
            Color color = renderPixel(x, y, scene);
            lineColors[x] = color;
        }
        {
            std::lock_guard<std::mutex> lock(screenMutex);
            for (int x = 0; x < width; x++) {
                _screen.setPixel(x, y, lineColors[x]);
            }
            _screen.setLineCompleted(y);
        }
    }


    void RayCaster::renderLoop(Scene &scene)
    {
        _renderWorkerPool = std::make_unique<RenderWorkerPool> (*this, scene);
        _renderWorkerPool->startWorkers();
    }

    void RayCaster::saveImage(const std::string& filename) const
    {
        std::cout << "Saving image in " << filename << std::endl;
        _screen.saveToPPM(filename);
        std::cout << "Image saved successfully." << std::endl;
    }

}
