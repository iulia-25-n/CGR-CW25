#pragma once
#include <string>
#include <vector>

struct Pixel { unsigned char r, g, b; };
struct Image {
    int w = 0, h = 0;
    std::vector<Pixel> data;
    Image() {}
    Image(int W, int H) :w(W), h(H), data(W* H) {}

    Pixel& at(int x, int y) { return data[y * w + x]; }
    const Pixel& at(int x, int y) const { return data[y * w + x]; }

    bool write_ppm(const std::string& path) const; // P3
    bool read_ppm(const std::string& path);        // P3
};

