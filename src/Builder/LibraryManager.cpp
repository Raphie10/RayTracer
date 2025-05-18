/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** LibraryManager
*/

#include "LibraryManager.hpp"
#include <filesystem>

namespace RayTracer {

    LibraryManager::LibraryManager()
    {
        loadLightLibrary();
        loadPrimitiveLibrary();
    }

    LibraryManager::~LibraryManager()
    {
        clear();
    }


    std::unique_ptr<DynamicLibrary> &LibraryManager::getCurrentLibrary(std::string libName, std::string functionName)
    {
        for (auto &lib : libraryHandles) {
            if (libName == lib.second->getName(functionName)) {
                std::unique_ptr<DynamicLibrary> &library = lib.second;
                return library;
            }
        }
        throw (std::runtime_error("Couldn't find primitive" + libName));
    }

    std::unique_ptr<DynamicLibrary> &LibraryManager::getCurrentLightLibrary(std::string libName, std::string functionName)
    {
        for (auto &lib : lightLibraries) {
            if (libName == lib.second->getName(functionName)) {
                std::unique_ptr<DynamicLibrary> &library = lib.second;
                return library;
            }
        }
        throw std::runtime_error("Couldn't find light " + libName);
    }

    bool LibraryManager::loadPrimitiveLibrary()
    {
        std::filesystem::path libPath = "Plugins/Primitives/";

        for (const auto& entry : std::filesystem::directory_iterator(libPath)) {
            if (entry.path().extension() == ".so") {
                std::string libName = entry.path().filename().string();
                std::string typeName = libName;
                if (typeName.substr(0, 3) == "lib") {
                    typeName = typeName.substr(3);
                }
                if (typeName.size() > 3 && typeName.substr(typeName.size() - 3) == ".so") {
                    typeName = typeName.substr(0, typeName.size() - 3);
                }
                libraryHandles[typeName] = std::make_unique<DynamicLibrary>(libPath / libName);
            }
        }
        return true;
    }

    void LibraryManager::loadLightLibrary()
    {
        std::filesystem::path libPath = "Plugins/Lights/";

        for (const auto& entry : std::filesystem::directory_iterator(libPath)) {
            if (entry.path().extension() == ".so") {
                std::string libName = entry.path().filename().string();
                std::string typeName = libName;
                if (typeName.substr(0, 3) == "lib") {
                    typeName = typeName.substr(3);
                }
                if (typeName.size() > 3 && typeName.substr(typeName.size() - 3) == ".so") {
                    typeName = typeName.substr(0, typeName.size() - 3);
                }
                lightLibraries[typeName] = std::make_unique<DynamicLibrary>(libPath / libName);
            }
        }
    }

}