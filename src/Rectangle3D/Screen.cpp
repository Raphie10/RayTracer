/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Screen.cpp
*/

#include "Screen.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

namespace RayTracer {

    Screen::Screen(int width, int height) : width(width), height(height)
    {
        pixels.resize(width * height, Color(0, 0, 0));
    }

    Screen::Screen(ArgumentMap params) : width(params["width"].as<int>()), height(params["height"].as<int>())
    {
        pixels.resize(width * height, Color(0, 0, 0));
        completedLines.resize(height, false);
    }

    Color Screen::getPixel(int x, int y) const
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return Color(0, 0, 0);
        return pixels[y * width + x];
    }

    void Screen::setPixel(int x, int y, const Color& color)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        double r = std::max(0.0, std::min(1.0, color.R));
        double g = std::max(0.0, std::min(1.0, color.G));
        double b = std::max(0.0, std::min(1.0, color.B));
        pixels[y * width + x] = Color(r, g, b);
    }

    void Screen::setPixel(int n, const Color& color)
    {
        if (n < 0 || n >= height * width)
            return;
        double r = std::max(0.0, std::min(1.0, color.R));
        double g = std::max(0.0, std::min(1.0, color.G));
        double b = std::max(0.0, std::min(1.0, color.B));
        pixels[n] = Color(r, g, b);
    }

    void Screen::getUV(int x, int y, double& u, double& v) const
    {
        u = static_cast<double>(x) / (width - 1);
        v = 1.0 - static_cast<double>(y) / (height - 1);
    }

    bool Screen::saveToPPM(const std::string& filename) const
    {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
            return false;
        }
        file << "P3\n" << width << " " << height << "\n255\n";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Color color = getPixel(x, y);
                int r = static_cast<int>(color.R * 255.999);
                int g = static_cast<int>(color.G * 255.999);
                int b = static_cast<int>(color.B * 255.999);
                file << r << " " << g << " " << b << "\n";
            }
        }
        file.close();
        return true;
    }

}
