#pragma once
#include "shape.h"
#include <cmath>

class Plane : public Shape {
public:
    Vec3 p;   // point on plane
    Vec3 n;   // normal(ised)

    Plane(const Vec3& P = { 0,0,0 }, const Vec3& N = { 0,1,0 }) : p(P), n(N) {}

    bool intersect(const Ray& ray, Hit& hit) const override {
        double denom = n.x * ray.d.x + n.y * ray.d.y + n.z * ray.d.z;
        if (std::fabs(denom) < 1e-6) return false;  // parallel
        double t = ((p.x - ray.o.x) * n.x + (p.y - ray.o.y) * n.y + (p.z - ray.o.z) * n.z) / denom;
        if (t < 0) return false;
        hit.t = t;
        hit.p = { ray.o.x + t * ray.d.x,
                  ray.o.y + t * ray.d.y,
                  ray.o.z + t * ray.d.z };
        hit.n = n;
        return true;
    }

    // bounding box
    AABB bounds() const override {
        // infinite planes => big box
        return AABB(Vec3(-1e5, -1e5, -1e5), Vec3(1e5, 1e5, 1e5));
    }
};

