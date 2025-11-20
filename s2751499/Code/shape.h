#pragma once
#include "camera.h"
#include "aabb.h"

struct Hit {
    double t = 1e30;
    Vec3 p;       // hit point
    Vec3 n;       // normal (world)
};

struct Shape {
    virtual ~Shape() {}
    virtual bool intersect(const Ray& r, Hit& h) const = 0;
    virtual AABB bounds() const = 0; 
};
