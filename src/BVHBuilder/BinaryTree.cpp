/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** BinaryTree
*/

#include "BinaryTree.hpp"
#include <stack>

namespace RayTracer {
    Node::Node(std::vector<std::unique_ptr<IPrimitive>> items)
    {
        if (items.empty())
            return;
        boundingBox = AABB({0, 0, 0}, {0, 0, 0});
        for (auto &item : items) {
            boundingBox = boundingBox + item->getBoundingBox();
        }
        if (items.size() <= 2) {
            objects = std::move(items);
            return;
        }
        Math::Point3D min;
        Math::Point3D max;
        min = items[0]->getBoundingBox().getCentroid();
        max = items[0]->getBoundingBox().getCentroid();
        for (auto &item : items) {
            Math::Point3D centroid = item->getBoundingBox().getCentroid();
            if (centroid.getX() < min.getX())
                min.setX(centroid.getX());
            if (centroid.getY() < min.getY())
                min.setY(centroid.getY());
            if (centroid.getZ() < min.getZ())
                min.setZ(centroid.getZ());
            if (centroid.getX() > max.getX())
                max.setX(centroid.getX());
            if (centroid.getY() > max.getY())
                max.setY(centroid.getY());
            if (centroid.getZ() > max.getZ())
                max.setZ(centroid.getZ());
        }
        Math::Vector3D extent = max - min;
        int axis = 0;
        if (extent.getY() > extent.getX())
            axis = 1;
        if (axis == 1)
            if (extent.getZ() > extent.getY())
                axis = 2;
        if (axis == 0)
            if (extent.getZ() > extent.getX())
                axis = 2;
        std::sort(items.begin(), items.end(), [axis](const std::unique_ptr<IPrimitive> &a, const std::unique_ptr<IPrimitive> &b) {
            Math::Point3D centerA = a->getBoundingBox().getCentroid();
            Math::Point3D centerB = b->getBoundingBox().getCentroid();
            switch (axis) {
            case 0:
                return centerA.getX() < centerB.getX();
            case 1:
                return centerA.getY() < centerB.getY();
            case 2:
            default:
                return centerA.getZ() < centerB.getZ();
            }
        });
        size_t middle = items.size() / 2;
        std::vector<std::unique_ptr<IPrimitive>> leftList;
        std::vector<std::unique_ptr<IPrimitive>> rightList;

        leftList.reserve(middle);
        rightList.reserve(items.size() - middle);

        std::move(std::make_move_iterator(items.begin()),
            std::make_move_iterator(items.begin() + middle),
            std::back_inserter(leftList));

        std::move(std::make_move_iterator(items.begin() + middle),
                std::make_move_iterator(items.end()),
                std::back_inserter(rightList));

        left = std::make_unique<Node>(std::move(leftList));
        right = std::make_unique<Node>(std::move(rightList));
    }

    // HitInfo Node::intersects(const Ray &ray) const
    // {
    //     HitInfo hitInfo;
    //     hitInfo.hit = false;
    //     if (!boundingBox.calculateEntryPoint(ray)) {
    //         return hitInfo;
    //     }
    //     if (isLeaf()) {
    //         HitInfo closestHit;
    //         closestHit.hit = false;
    //         for (const auto& primitive : objects) {
    //             HitInfo hit = primitive->intersect(ray);
    //             if (hit.hit && (!closestHit.hit || hit.distance < closestHit.distance)) {
    //                 closestHit = hit;
    //             }
    //         }
    //         return closestHit;
    //     }
    //     HitInfo leftHit = left->intersects(ray);
    //     HitInfo rightHit = right->intersects(ray);
    //     if (leftHit.hit && rightHit.hit)
    //         if (leftHit.distance < rightHit.distance)
    //             return leftHit;
    //         else return rightHit;
    //     else if (leftHit.hit) {
    //         return leftHit;
    //     } else
    //         return rightHit;
    // }

    HitInfo Node::intersects(const Ray &ray) const
    {
        HitInfo closestHit;
        std::stack<const Node *> nodes;
        closestHit.hit = false;
        nodes.push(this);
        while (!nodes.empty()) {
            const Node *currentNode = nodes.top();
            nodes.pop();
            if (!currentNode->boundingBox.calculateEntryPoint(ray)) {
                continue;
            }
            if (currentNode->isLeaf()) {
                for (const auto& primitive : currentNode->objects) {
                    HitInfo hit = primitive->intersect(ray);
                    if (hit.hit && (!closestHit.hit || hit.distance < closestHit.distance)) {
                        closestHit = hit;
                    }
                }
            } else {
                if (currentNode->left) {
                    nodes.push(currentNode->left.get());
                }
                if (currentNode->right) {
                    nodes.push(currentNode->right.get());
                }
            }
        }
        return closestHit;
        // HitInfo leftHit = left->intersects(ray);
        // HitInfo rightHit = right->intersects(ray);
        // if (leftHit.hit && rightHit.hit)
        //     if (leftHit.distance < rightHit.distance)
        //         return leftHit;
        //     else return rightHit;
        // else if (leftHit.hit) {
        //     return leftHit;
        // } else
        //     return rightHit;
    }

}