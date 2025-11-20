#pragma once
#include "camera.h"
#include <algorithm>

struct AABB {
    Vec3 min;
    Vec3 max;

    AABB() {}
    AABB(const Vec3& a, const Vec3& b) : min(a), max(b) {}

    // Expand box to include another
    void expand(const AABB& box) {
        min.x = std::min(min.x, box.min.x);
        min.y = std::min(min.y, box.min.y);
        min.z = std::min(min.z, box.min.z);
        max.x = std::max(max.x, box.max.x);
        max.y = std::max(max.y, box.max.y);
        max.z = std::max(max.z, box.max.z);
    }

    // Check intersection with a ray (returns true/false)
    bool intersect(const Ray& ray, double& tmin, double& tmax) const {
        tmin = -1e9;
        tmax = 1e9;

        for (int i = 0; i < 3; i++) {
            double invD = 1.0 / ((i == 0) ? ray.d.x : (i == 1) ? ray.d.y : ray.d.z);
            double t0 = (((i == 0) ? min.x : (i == 1) ? min.y : min.z) -
                ((i == 0) ? ray.o.x : (i == 1) ? ray.o.y : ray.o.z)) * invD;
            double t1 = (((i == 0) ? max.x : (i == 1) ? max.y : max.z) -
                ((i == 0) ? ray.o.x : (i == 1) ? ray.o.y : ray.o.z)) * invD;

            if (invD < 0.0) std::swap(t0, t1);
            tmin = std::max(tmin, t0);
            tmax = std::min(tmax, t1);

            if (tmax <= tmin) return false;
        }
        return true;
    }
};
