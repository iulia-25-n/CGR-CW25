#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "camera.h"
#include "image.h"
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "bvh.h"

int main() {
    std::cout << "Module 2\n";

    // load camera
    Camera cam;
    if (!cam.load_from_ascii("../ASCII/scene.txt")) {
        std::cerr << "Failed to read ASCII.\n";
        return 1;
    }
    cam.finalize();
    std::cout << "Camera loaded at (" << cam.pos.x << ", "
        << cam.pos.y << ", " << cam.pos.z << ")\n";

    // create scene - test
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Sphere>(Vec3{ 5, -3, 2 }, 2.0));         // at origin
    shapes.push_back(std::make_shared<Plane>(Vec3{ 0, -2, 0 }, Vec3{ 0, 1, 0 })); // ground plane
    shapes.push_back(std::make_shared<Cube>(Vec3{ -3, -3, 5 }, 2.0));       // cube side
    std::shared_ptr<BVHNode> root = build_bvh(shapes, 0, (int)shapes.size()); // BVH hierarchy just once after render

    // output image
    Image img(cam.resx, cam.resy);
    // start clock
    auto start = std::chrono::high_resolution_clock::now();
    // raytracing main
    for (int y = 0; y < cam.resy; y++) {
        for (int x = 0; x < cam.resx; x++) {
            Ray r = cam.pixel_to_world_ray(x, y);

            Hit closestHit;
            bool hitSomething = false;
            double minT = 1e9;
            // traverses BVH tree recursively
            hitSomething = bvh_intersect(root, r, closestHit, minT);

            if (hitSomething) {
                // normal to RGB
                unsigned char R = (unsigned char)((closestHit.n.x * 0.5 + 0.5) * 255);
                unsigned char G = (unsigned char)((closestHit.n.y * 0.5 + 0.5) * 255);
                unsigned char B = (unsigned char)((closestHit.n.z * 0.5 + 0.5) * 255);
                img.at(x, y) = Pixel{ R, G, B };
            }
            else {
                // bg
                img.at(x, y) = Pixel{ 100, 150, 255 };
            }
        }
    }
    // stop clock
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Render time: " << elapsed.count() << " seconds\n";

    // output
    if (!img.write_ppm("../Output/test.ppm")) {
        std::cerr << "Failed output.\n";
        return 1;
    }

    std::cout << "Succes - wrote output.\n";
    return 0;
}
