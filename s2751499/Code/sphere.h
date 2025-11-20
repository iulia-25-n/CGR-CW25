#pragma once
#include "shape.h"
#include <cmath>

class Sphere : public Shape {
public:
    Vec3 c;
    double r;

    Sphere(const Vec3& C = { 0,0,0 }, double R = 1.0) : c(C), r(R) {}

    bool intersect(const Ray& ray, Hit& hit) const override {
        Vec3 oc = ray.o - c;
        double b = 2.0 * (oc.x * ray.d.x + oc.y * ray.d.y + oc.z * ray.d.z);
        double c_ = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - r * r;
        double disc = b * b - 4.0 * c_;
        if (disc < 0.0) return false;

        double t = (-b - std::sqrt(disc)) / 2.0;
        if (t < 0) t = (-b + std::sqrt(disc)) / 2.0;
        if (t < 0) return false;

        hit.t = t;
        hit.p = { ray.o.x + ray.d.x * t,
                  ray.o.y + ray.d.y * t,
                  ray.o.z + ray.d.z * t };
        Vec3 n = { hit.p.x - c.x, hit.p.y - c.y, hit.p.z - c.z };
        double len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        if (len > 1e-8) { n.x /= len; n.y /= len; n.z /= len; }
        hit.n = n;
        return true;
    }

    AABB bounds() const override {
        Vec3 e(r, r, r);
        return AABB(c - e, c + e);
    }
};

