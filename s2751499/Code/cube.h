#pragma once
#include "shape.h"
#include <cmath>

class Cube : public Shape {
public:
    Vec3 c;  // centre
    double s; // half-size

    Cube(const Vec3& C = { 0,0,0 }, double S = 1.0) : c(C), s(S) {}

    bool intersect(const Ray& ray, Hit& hit) const override {
        Vec3 invD = { 1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z };
        Vec3 t0 = (c - Vec3(s, s, s) - ray.o) * invD;
        Vec3 t1 = (c + Vec3(s, s, s) - ray.o) * invD;
        Vec3 tmin = { std::fmin(t0.x, t1.x), std::fmin(t0.y, t1.y), std::fmin(t0.z, t1.z) };
        Vec3 tmax = { std::fmax(t0.x, t1.x), std::fmax(t0.y, t1.y), std::fmax(t0.z, t1.z) };
        double tNear = std::fmax(tmin.x, std::fmax(tmin.y, tmin.z));
        double tFar = std::fmin(tmax.x, std::fmin(tmax.y, tmax.z));
        if (tNear > tFar || tFar < 0) return false;
        hit.t = (tNear > 0) ? tNear : tFar;
        hit.p = ray.o + ray.d * hit.t;
        // normal
        Vec3 normal(0, 0, 0);
        Vec3 local = hit.p - c;
        double bias = 1e-4;
        if (std::fabs(local.x - s) < bias) normal = { 1,0,0 };
        else if (std::fabs(local.x + s) < bias) normal = { -1,0,0 };
        else if (std::fabs(local.y - s) < bias) normal = { 0,1,0 };
        else if (std::fabs(local.y + s) < bias) normal = { 0,-1,0 };
        else if (std::fabs(local.z - s) < bias) normal = { 0,0,1 };
        else if (std::fabs(local.z + s) < bias) normal = { 0,0,-1 };
        hit.n = normal;
        return true;
    }

    // bounding
    AABB bounds() const override {
        Vec3 e(s, s, s);
        return AABB(c - e, c + e);
    }
};

