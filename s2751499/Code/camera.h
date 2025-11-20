#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

struct Vec3 {
    double x = 0, y = 0, z = 0;
    Vec3() {}
    Vec3(double X, double Y, double Z) :x(X), y(Y), z(Z) {}
    Vec3 operator+(const Vec3& o)const { return { x + o.x,y + o.y,z + o.z }; }
    Vec3 operator-(const Vec3& o)const { return { x - o.x,y - o.y,z - o.z }; }
    Vec3 operator*(double s)const { return { x * s,y * s,z * s }; }
    Vec3 operator*(const Vec3& o) const { return { x * o.x, y * o.y, z * o.z }; }
};
inline Vec3 normalize(const Vec3& v) {
    double n = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return (n > 1e-12) ? Vec3(v.x / n, v.y / n, v.z / n) : v;
}

struct Mat3 { double m[3][3] = { {1,0,0},{0,1,0},{0,0,1} }; };
inline Vec3 mul(const Mat3& M, const Vec3& v) {
    return {
        M.m[0][0] * v.x + M.m[0][1] * v.y + M.m[0][2] * v.z,
        M.m[1][0] * v.x + M.m[1][1] * v.y + M.m[1][2] * v.z,
        M.m[2][0] * v.x + M.m[2][1] * v.y + M.m[2][2] * v.z
    };
}
Mat3 eulerXYZ_to_mat3(double rx, double ry, double rz);

struct Ray { Vec3 o, d; };

struct Camera {
    Vec3 pos;            // world pos
    Vec3 euler;          // rx, ry, rz (radians, Blender XYZ)
    double focal_mm = 50;  // mm
    double sensor_w = 36;
    double sensor_h = 24;
    int resx = 640, resy = 480;

    Mat3 R;              // rotation: world = R * local
    double px_size_x = 0;
    double px_size_y = 0;

    bool load_from_ascii(const std::string& path);
    void finalize();
    Ray pixel_to_world_ray(double px, double py) const;
};

