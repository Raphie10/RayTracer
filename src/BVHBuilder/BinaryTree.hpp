/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** BinaryTree
*/

#pragma once

#include <memory>
#include <vector>
#include "IPrimitive.hpp"
#include "AABB.hpp"
#include <algorithm>
#include "../Ray/Ray.hpp"

namespace RayTracer {
    class Node {
        public:
            Node(std::unique_ptr<IPrimitive> item)
            {
                objects.push_back(std::move(item));
            }
            Node(std::vector<std::unique_ptr<IPrimitive>> items);

            bool isLeaf() const { return !right && !left; }

            std::vector<HitInfo> intersects(const Ray &ray) const;
            AABB getBoundingBox() const { return boundingBox; }

            ~Node() = default;

        private:
            class BinaryTreeError : public std::exception {
                public:
                    BinaryTreeError(std::string errMessage) { errorMessage = errMessage; }
                    const char *what() const noexcept override { return errorMessage.c_str(); }
                private:
                    std::string errorMessage;
            };
            AABB boundingBox;
            std::unique_ptr<Node> left = nullptr;
            std::unique_ptr<Node> right = nullptr;
            std::vector<std::unique_ptr<IPrimitive>> objects;
    };
}

