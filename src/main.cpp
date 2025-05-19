/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include "../Builder/RayTracerApp.hpp"
#include <any>


int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.cfg>" << std::endl;
        return 1;
    }
    // try {
        std::string configFile = argv[1];
        std::unique_ptr<RayTracer::RayTracerApp> app = std::make_unique<RayTracer::RayTracerApp>(configFile);
        app->run();
    // } catch (std::bad_any_cast &e) {
    //     std::cerr << "Error: Bad any cast" << std::endl;
    //     exit(1);
    // } catch (std::exception &e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }
    return 0;
}