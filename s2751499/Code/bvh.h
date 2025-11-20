#pragma once
#include <memory>
#include <vector>
#include "shape.h"
#include "aabb.h"

struct BVHNode {
    AABB box;
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;
    std::shared_ptr<Shape> shape; // leaf if not null
};

// recursive build bvh from a list of shapes
inline std::shared_ptr<BVHNode> build_bvh(std::vector<std::shared_ptr<Shape>>& shapes, int start, int end) {
    auto node = std::make_shared<BVHNode>();

    if (end - start == 1) {
        // leaf node
        node->shape = shapes[start];
        node->box = shapes[start]->bounds();
        return node;
    }

    // compute bounding box of all shapes
    AABB globalBox = shapes[start]->bounds();
    for (int i = start + 1; i < end; i++)
        globalBox.expand(shapes[i]->bounds());
    node->box = globalBox;

    // simple split by index midpoint
    int mid = (start + end) / 2;
    node->left = build_bvh(shapes, start, mid);
    node->right = build_bvh(shapes, mid, end);

    return node;
}

// recursive intersection test
inline bool bvh_intersect(const std::shared_ptr<BVHNode>& node, const Ray& ray, Hit& bestHit, double& minT) {
    double tmin = 0.001, tmax = 1e9;
    if (!node->box.intersect(ray, tmin, tmax)) return false;

    if (node->shape) {
        Hit h;
        if (node->shape->intersect(ray, h) && h.t < minT) {
            minT = h.t;
            bestHit = h;
            return true;
        }
        return false;
    }

    bool hitLeft = false, hitRight = false;
    if (node->left) hitLeft = bvh_intersect(node->left, ray, bestHit, minT);
    if (node->right) hitRight = bvh_intersect(node->right, ray, bestHit, minT);
    return hitLeft || hitRight;
}
