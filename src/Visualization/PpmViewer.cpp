/*
** EPITECH PROJECT, 2024
** RayTracer
** File description:
** PpmViewer.cpp
*/

#include "PpmViewer.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "../Builder/RayTracerApp.hpp"

namespace RayTracer {

    PpmViewer::PpmViewer(const std::string& ppmFile, RayTracerApp &raytracerApp, int width, int height)
        : _rayTracerApp(raytracerApp),
        lastRenderedLine(0),
        displayActive(false),
        windowWidth(width),
        windowHeight(height)
    {
        if (!ppmFile.empty()) {
            loadPpmFile(ppmFile);
        }
        displayImage.create(windowWidth, windowHeight, sf::Color::Black);
    }

    PpmViewer::~PpmViewer()
    {
        stopDisplay();
        if (displayThread.joinable()) {
            displayThread.join();  // Ensure thread exits before SFML objects get destroyed
        }
    }

    bool PpmViewer::loadPpmFile(const std::string& filePath)
    {
        ppmFilePath = filePath;
        return parsePpm();
    }

    bool PpmViewer::parsePpm()
    {
        std::ifstream file(ppmFilePath);
        if (!file.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier PPM " << ppmFilePath << std::endl;
            return false;
        }
        std::string format;
        int width, height, maxVal;
        file >> format;
        char c;
        file.get(c);
        while (c == '\n' || c == ' ') {
            file.get(c);
            if (c == '#') {
                std::string comment;
                std::getline(file, comment);
                file.get(c);
            }
        }
        file.unget();
        file >> width >> height >> maxVal;
        if (format != "P3") {
            std::cerr << "Format PPM non supporté: " << format << std::endl;
            file.close();
            return false;
        }

        sf::Image image;
        image.create(width, height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r, g, b;
                file >> r >> g >> b;
                image.setPixel(x, y, sf::Color(r, g, b));
            }
        }

        file.close();
        texture.loadFromImage(image);
        sprite.setTexture(texture, true);
        if (windowWidth == 0 || windowHeight == 0) {
            windowWidth = width;
            windowHeight = height;
        }
        return true;
    }

    void PpmViewer::start_rendering()
    {
        // if (displayActive.load()) {
        //     return;
        // }
        // stopDisplay();
        lastRenderedLine = 0;
        displayActive.store(true);

        if (!window.isOpen()) {
            window.create(sf::VideoMode(windowWidth, windowHeight), "RayTracer - SFML Viewer");
        }

        displayImage.create(windowWidth, windowHeight, sf::Color::Black);
        texture.create(windowWidth, windowHeight);
        texture.update(displayImage);
        sprite.setTexture(texture, true);
        if (displayThread.joinable()) {
            displayThread.join();
        }
        displayActive.store(true);
        displayThread = std::thread(&PpmViewer::displayLoop, this);
    }

    void PpmViewer::stopDisplay()
    {
        displayActive.store(false);
        if (displayThread.joinable())
            displayThread.join();
        if (window.isOpen())
            window.close();
    }

    void PpmViewer::updateTexture()
    {
        if (!displayActive.load()) {
            return;
        }

        const Screen& screen =_rayTracerApp.getRayCaster()->getScreen();
        int width = screen.getWidth();
        int height = screen.getHeight();
        int currentLine = _rayTracerApp.getRayCaster()->getCurrentLine();

        {
            std::lock_guard<std::mutex> lock(displayMutex);
            for (int y = lastRenderedLine; y < currentLine && y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    Color color = screen.getPixel(x, y);
                    // couleur de [0,1] à [0,255]
                    sf::Color pixelColor(
                        static_cast<sf::Uint8>(std::min(255.0, color.R * 255)),
                        static_cast<sf::Uint8>(std::min(255.0, color.G * 255)),
                        static_cast<sf::Uint8>(std::min(255.0, color.B * 255))
                    );
                    displayImage.setPixel(x, y, pixelColor);
                }
            }
            if (lastRenderedLine < currentLine) {
                texture.update(displayImage);
                lastRenderedLine = currentLine;
            }
        }
    }

    bool PpmViewer::isMovementKey(sf::Keyboard::Key key) const
    {
        return key == sf::Keyboard::Z || key == sf::Keyboard::Q || key == sf::Keyboard::S || key == sf::Keyboard::D
        || key == sf::Keyboard::Space || key == sf::Keyboard::LShift || key == sf::Keyboard::RShift;
    }

    void PpmViewer::darkenImage(sf::Image &image, float factor)
    {
        if (factor < 0.0f || factor > 1.0f)
            return;

        sf::Vector2u size = image.getSize();

        for (unsigned int y = 0; y < size.y; ++y) {
            for (unsigned int x = 0; x < size.x; ++x) {
                sf::Color pixel = image.getPixel(x, y);

                pixel.r = static_cast<sf::Uint8>(pixel.r * factor);
                pixel.g = static_cast<sf::Uint8>(pixel.g * factor);
                pixel.b = static_cast<sf::Uint8>(pixel.b * factor);

                image.setPixel(x, y, pixel);
            }
        }
    }

    void PpmViewer::manageMovementInput(const sf::Event &event)
    {
        std::shared_ptr<RayTracer::Scene> &scene = _rayTracerApp.getScene();
        Math::Vector3D forward = scene->getCameraDirection();
        Math::Vector3D right = scene->getCameraRight();
        Math::Vector3D up(0, 1, 0);
        double speed = 2.0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
            speed *= 4.0;
        }

        if (isMovementKey(event.key.code)) {
            switch (event.key.code) {
                case sf::Keyboard::Z:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(0, 0, speed) * forward);
                    break;
                case sf::Keyboard::Q:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(-speed, 0, 0) * right);
                    break;
                case sf::Keyboard::S:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(0, 0, -speed) * forward);
                    break;
                case sf::Keyboard::D:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(speed, 0, 0) * right);
                    break;
                case sf::Keyboard::Space:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(0, speed, 0) * up);
                    break;
                case sf::Keyboard::LShift:
                case sf::Keyboard::RShift:
                    _rayTracerApp.getScene()->moveCamera(Math::Vector3D(0, -speed, 0) * up);
                    break;
            }
            _rayTracerApp.getRayCaster()->restartRendering(_rayTracerApp.getScene());
            lastRenderedLine = 0;
        }
    }

    void PpmViewer::manageRotationInput(const sf::Event &event)
    {
        if (event.key.code == sf::Keyboard::Left) {
            _rayTracerApp.getScene()->changeCameraOrientation(Math::Vector3D(0, -5, 0));
        } else if (event.key.code == sf::Keyboard::Right) {
            _rayTracerApp.getScene()->changeCameraOrientation(Math::Vector3D(0, 5, 0));
        } else if (event.key.code == sf::Keyboard::Up) {
            _rayTracerApp.getScene()->changeCameraOrientation(Math::Vector3D(5, 0, 0));
        } else if (event.key.code == sf::Keyboard::Down) {
            _rayTracerApp.getScene()->changeCameraOrientation(Math::Vector3D(-5, 0, 0));
        }
        _rayTracerApp.getRayCaster()->restartRendering(_rayTracerApp.getScene());
        lastRenderedLine = 0;
    }

    void PpmViewer::displayLoop()
    {
        float scaleX = static_cast<float>(windowWidth) / displayImage.getSize().x;
        float scaleY = static_cast<float>(windowHeight) / displayImage.getSize().y;
        sprite.setScale(scaleX, scaleY);

        displayActive.store(true);
        while (window.isOpen() && displayActive.load()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _rayTracerApp.getRayCaster()->stopRendering();
                    displayActive.store(false);
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        darkenImage(displayImage, 0.5f);
                        _rayTracerApp.getRayCaster()->restartRendering(_rayTracerApp.getScene());
                        lastRenderedLine = 0;
                    }
                    manageMovementInput(event);
                    manageRotationInput(event);
                }
            }

            updateTexture();
            const auto& completed = _rayTracerApp.getRayCaster()->getScreen().getCompletedLines();
            int current = _rayTracerApp.getRayCaster()->getCurrentLine();

            bool renderingComplete = false;
            if (current >= 0 && current < static_cast<int>(completed.size())) {
                while (completed[current]) {
                    _rayTracerApp.getRayCaster()->setCurrentLine(++current);
                    if (current >= static_cast<int>(completed.size())) {
                        renderingComplete = true;
                        break;
                    }
                }
            }
            // window.clear(sf::Color::Black);
            window.draw(sprite);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
    }

}
