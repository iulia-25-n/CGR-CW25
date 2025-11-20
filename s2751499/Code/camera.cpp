#include "camera.h"
#include <fstream>
#include <sstream>
#include <string>

Mat3 eulerXYZ_to_mat3(double rx, double ry, double rz) {
    double cx = cos(rx), sx = sin(rx);
    double cy = cos(ry), sy = sin(ry);
    double cz = cos(rz), sz = sin(rz);
    Mat3 Rx{ {{1,0,0},{0,cx,-sx},{0,sx,cx}} };
    Mat3 Ry{ {{cy,0,sy},{0,1,0},{-sy,0,cy}} };
    Mat3 Rz{ {{cz,-sz,0},{sz,cz,0},{0,0,1}} };
    auto mul3 = [](const Mat3& A, const Mat3& B) {
        Mat3 C{};
        for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
            C.m[i][j] = 0;
            for (int k = 0; k < 3; k++) C.m[i][j] += A.m[i][k] * B.m[k][j];
        }
        return C;
    };
    // default Euler order XYZ -> Rx then Ry then Rz -> R = Rz*Ry*Rx
    return mul3(mul3(Rz, Ry), Rx);
}

bool Camera::load_from_ascii(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    std::string w;
    while (in >> w) {
        if (w == "CAMERA") {
            std::string tag;
            while (in >> tag) {
                if (tag == "location") { in >> pos.x >> pos.y >> pos.z; }
                else if (tag == "euler") { in >> euler.x >> euler.y >> euler.z; }
                else if (tag == "focal_sensor") { in >> focal_mm >> sensor_w >> sensor_h; }
                else if (tag == "resolution") { in >> resx >> resy; break; }
                else { std::string rest; std::getline(in, rest); } // skip extras
            }
            break;
        }
    }
    finalize();
    return true;
}
void Camera::finalize() {
    R = eulerXYZ_to_mat3(euler.x, euler.y, euler.z);
    px_size_x = sensor_w / double(resx);
    px_size_y = sensor_h / double(resy);
}
Ray Camera::pixel_to_world_ray(double px, double py) const {
    // pixel centre in sensor mm; +x right, +y up; camera looks -Z local
    double x_mm = (-sensor_w * 0.5) + (px + 0.5) * px_size_x;
    double y_mm = (sensor_h * 0.5) - (py + 0.5) * px_size_y;
    Vec3 dir_cam = normalize(Vec3(x_mm, y_mm, -focal_mm));
    Vec3 dir_world = normalize(mul(R, dir_cam));
    return { pos, dir_world };
}
