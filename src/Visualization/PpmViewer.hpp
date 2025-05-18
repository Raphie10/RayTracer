/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** PpmViewer.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include "../Camera/Camera.hpp"

namespace RayTracer {
    class RayTracerApp;

    class PpmViewer {
        private:
            sf::RenderWindow window;
            sf::Texture texture;
            sf::Sprite sprite;
            sf::Image displayImage;
            std::string ppmFilePath;
            int windowWidth;
            int windowHeight;

            std::mutex displayMutex;
            RayTracerApp &_rayTracerApp;
            int lastRenderedLine;
            std::atomic<bool> displayActive;
            std::thread displayThread;

        public:
            PpmViewer(const std::string& ppmFile, RayTracerApp &raytracerApp, int width, int height);
            ~PpmViewer();

            bool loadPpmFile(const std::string& filePath);
            bool parsePpm();

            void start_rendering();
            void stopDisplay();
            void displayLoop();
            void updateTexture();
            bool isMovementKey(sf::Keyboard::Key key) const;
            bool isDisplayActive() const { return displayActive.load(); }
            bool loadEndMusic(const std::string& musicFile);
            void darkenImage(sf::Image &image, float factor);
            void manageMovementInput(const sf::Event &event);
            void manageRotationInput(const sf::Event &event);
        };

}
