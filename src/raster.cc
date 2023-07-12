#include "raster.h"
#include <limits>




bool PointOnLineLeft3D(const Eigen::Vector3d& point, const Eigen::Vector3d& start, const Eigen::Vector3d& end) {
    Eigen::Vector3d line_vector = end - start;
    Eigen::Vector3d point_vector = point - start;
    line_vector.normalize();
    point_vector.normalize();
    return line_vector.cross(point_vector).z() > 0.0;
}

bool PointOnLineLeft(const Eigen::Vector2d& point, const Eigen::Vector4d& start, const Eigen::Vector4d& end) {
    Eigen::Vector3d p3d = point.homogeneous();
    Eigen::Vector3d start_3d = start.head<2>().homogeneous();
    Eigen::Vector3d end_3d = end.head<2>().homogeneous();
    return PointOnLineLeft3D(p3d,start_3d, end_3d);
}

std::vector<std::pair<int, int>> Raster::RasterNDCTriangleMask(const NDCTriangle &ndc_angle, size_t width, size_t height) {
    // row major
    std::vector<std::pair<int, int>> res;
    res.reserve(width * height);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            double x = (2.0 * col - width) / width;
            double y = (2.0 * row - height) / height;
            if (PointOnLineLeft(Eigen::Vector2d(x, y), ndc_angle.i(),
                                ndc_angle.j()) &&
                PointOnLineLeft(Eigen::Vector2d(x, y), ndc_angle.j(),
                                ndc_angle.k()) &&
                PointOnLineLeft(Eigen::Vector2d(x, y), ndc_angle.k(),
                                ndc_angle.i())) {
              res.push_back(std::make_pair(row, col));
            }
        }
    }
    return res;
}
