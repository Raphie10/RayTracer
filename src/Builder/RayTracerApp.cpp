/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** RayTracerApp
*/

#include "RayTracerApp.hpp"
#include "../Builder/RayTracer.hpp"
#include "../Visualization/PpmViewer.hpp"
#include "../Rectangle3D/Screen.hpp"
#include "../Builder/LibraryManager.hpp"
#include "../Builder/Scene.hpp"
#include "RayTracerApp.hpp"
#include <filesystem>
#include <memory>
#include <chrono>
#include <iomanip>

namespace RayTracer {

    RayTracerApp::~RayTracerApp()
    {
    }

    void RayTracerApp::loadConfigFile()
    {
        currentWriteTime = std::filesystem::last_write_time(_configFile);
        _scene.reset();
        _parser.emplace(Parsing_cfg(_configFile));
        _scene = std::make_shared<Scene> (*_parser, _libraryHandles);
        _raycaster = std::make_unique<RayCaster> (_parser->getResolution(), *_parser);
    }

    void RayTracerApp::run()
    {
        beginingTime = std::chrono::high_resolution_clock::now();
        loadConfigFile();

        _viewer = std::make_unique<PpmViewer>("", *this, _raycaster->getScreen().getWidth(), _raycaster->getScreen().getHeight());
        _raycaster->start_rendering(_scene);
        _viewer->start_rendering();
        while (!_raycaster->isRaytracingDone()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        displayTime();
        while (_viewer->isDisplayActive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (std::filesystem::last_write_time(_configFile) != currentWriteTime) {
                try {
                    loadConfigFile();
                    std::cout << "Reloading configuration file..." << std::endl;
                    _raycaster->restartRendering(_scene);
                    _viewer->resetLastRenderedLine();
                    currentWriteTime = std::filesystem::last_write_time(_configFile);
                } catch (...){}
            }
        }
        _viewer->stopDisplay();
        std::cout << "Rendering completed." << std::endl;
    }

    void RayTracerApp::displayTime()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginingTime);
        double seconds = duration.count() / 1000000.0;
        std::cout << "Rendering time: " << std::fixed << std::setprecision(3) << seconds << " s" << std::endl;
    }
}
