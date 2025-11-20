#include "image.h"
#include <fstream>

bool Image::write_ppm(const std::string& path) const {
    std::ofstream out(path);
    if (!out) return false;
    out << "P3\n" << w << " " << h << "\n255\n";
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            const auto& p = at(x, y);
            out << (int)p.r << " " << (int)p.g << " " << (int)p.b << " ";
        }
        out << "\n";
    }
    return true;
}
bool Image::read_ppm(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    std::string magic; in >> magic;
    if (magic != "P3") return false;
    int maxv; in >> w >> h >> maxv;
    data.assign(w * h, {});
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int r, g, b; in >> r >> g >> b;
            at(x, y) = Pixel{ (unsigned char)r,(unsigned char)g,(unsigned char)b };
        }
    return true;
}
