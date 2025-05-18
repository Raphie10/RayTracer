/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** LibraryManager
*/

#pragma once

#include <unordered_map>
#include <iostream>
#include <memory>
#include "../DynamicLibrary/DynamicLibrary.hpp"

namespace RayTracer {

    class LibraryManager {
        public:
            LibraryManager();
            ~LibraryManager();
            std::unique_ptr<DynamicLibrary> &getCurrentLibrary(std::string libName, std::string functionName);
            std::unique_ptr<DynamicLibrary> &getCurrentLightLibrary(std::string libName, std::string functionName);
            bool loadPrimitiveLibrary();
            void loadLightLibrary();
            void clear()
            {
                libraryHandles.clear();
                lightLibraries.clear();
            }
        private:
            std::unordered_map<std::string, std::unique_ptr<DynamicLibrary>> libraryHandles;
            std::unordered_map<std::string, std::unique_ptr<DynamicLibrary>> lightLibraries;
    };
}