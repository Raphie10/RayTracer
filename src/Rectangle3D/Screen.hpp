/*
** EPITECH PROJECT, 2024
** Zero
** File description:
** Screen.hpp
*/

#pragma once

#include "Vector.hpp"
#include "Points.hpp"
#include <vector>
#include <string>
#include "../Parsing/ArgumentMap.hpp"
#include "Color.hpp"
#include <algorithm>

namespace RayTracer {

    class Screen {
        private:
            int width;
            int height;
            double pixelWidth;
            double pixelHeight;
            std::vector<Color> pixels;
            std::vector<bool> completedLines;
        public:
            Screen(int width = 800, int height = 600);
            Screen(ArgumentMap params);
            int getWidth() const { return width; }
            int getHeight() const { return height; }
            Color getPixel(int x, int y) const;
            void setPixel(int x, int y, const Color& color);
            void setPixel(int n, const Color& color);
            int getPixelWidth() const { return pixelWidth; }
            int getPixelHeight() const { return pixelHeight; }
            void getUV(int x, int y, double& u, double& v) const;
            bool saveToPPM(const std::string& filename) const;
            const std::vector<bool> &getCompletedLines() const {return completedLines;}
            void setLineCompleted(int y) {completedLines[y] = true;}
            void resetCompletedLines() {completedLines.assign(height, false); }
            int getCompletedLineCount() const {
                return std::count(completedLines.begin(), completedLines.end(), true);
            }
    };
}