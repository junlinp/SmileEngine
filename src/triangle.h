#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_
#include <array>

#include "Eigen/Dense"

class Triangle {
    private:
    // counter-clockwise
   Eigen::Vector3d vertex_i_, vertex_j_, vertex_k_; 
public:
    Triangle(const Eigen::Vector3d& vertex_i, const Eigen::Vector3d& vertex_j, const Eigen::Vector3d& vertex_k) :
    vertex_i_(vertex_i), vertex_j_(vertex_j), vertex_k_(vertex_k) {}

   std::array<double, 9> ExportVertexData() {
    std::array<double, 9> res;
    std::copy(vertex_i_.data(), vertex_i_.data() + 3, res.data());
    std::copy(vertex_j_.data(), vertex_j_.data() + 3, res.data() + 3);
    std::copy(vertex_k_.data(), vertex_k_.data() + 3, res.data() + 9);
    return res;
   }
   Eigen::Vector3d i() const { return vertex_i_;}
   Eigen::Vector3d j() const { return vertex_j_;}
   Eigen::Vector3d k() const { return vertex_k_;}
};

class NDCTriangle {
    private:
    // counter-clockwise
   Eigen::Vector4d vertex_i_, vertex_j_, vertex_k_; 
    Eigen::Vector3d color_rgb_i_, color_rgb_j_, color_rgb_k_;
    double alpha_i_ = 0.75;
    double alpha_j_ = 0.75;
    double alpha_k_ = 0.75;
public:
    NDCTriangle(const Eigen::Vector4d& vertex_i,const Eigen::Vector4d& vertex_j, const Eigen::Vector4d& vertex_k) :
    vertex_i_(vertex_i), vertex_j_(vertex_j), vertex_k_(vertex_k), color_rgb_i_(1.0, 0.0, 0.0), color_rgb_j_(0.0, 1.0, 0.0), color_rgb_k_(0.0, 0.0, 1.0) {}
   Eigen::Vector4d i() const { return vertex_i_;}
   Eigen::Vector4d j() const { return vertex_j_;}
   Eigen::Vector4d k() const { return vertex_k_;}

   std::array<double, 9> ExportVertexData() {
    std::array<double, 9> res;
    std::copy(vertex_i_.data(), vertex_i_.data() + 3, res.data());
    std::copy(vertex_j_.data(), vertex_j_.data() + 3, res.data() + 3);
    std::copy(vertex_k_.data(), vertex_k_.data() + 3, res.data() + 9);
    return res;
   }

    double TriangleArea(const Eigen::Vector2d& start, const Eigen::Vector2d& end_1, const Eigen::Vector2d& end_2) const {
        return 0.5 * (end_1.homogeneous() - start.homogeneous()).cross(end_2.homogeneous() - start.homogeneous()).norm();
    }

   Eigen::Vector3d PerspectiveInterpolation(const Eigen::Vector2d&xy) const {
    Eigen::Vector2d vertex_i_xy = vertex_i_.head<2>();
    Eigen::Vector2d vertex_j_xy = vertex_j_.head<2>();
    Eigen::Vector2d vertex_k_xy = vertex_k_.head<2>();
    double total_weight = TriangleArea(vertex_i_xy, vertex_j_xy, vertex_k_xy);
    double weight_i = TriangleArea(xy, vertex_j_xy, vertex_k_xy) / total_weight;
    double weight_j = TriangleArea(xy, vertex_i_xy, vertex_k_xy) / total_weight;
    double weight_k = TriangleArea(xy, vertex_j_xy, vertex_i_xy) / total_weight;
    double z_depth_i = vertex_i_.w();
    double z_depth_j = vertex_j_.w();
    double z_depth_k = vertex_k_.w();

    return (color_rgb_i_ * weight_i / z_depth_i + color_rgb_j_ * weight_j / z_depth_j + color_rgb_k_ * weight_k / z_depth_k) / (weight_i / z_depth_i + weight_j / z_depth_j + weight_k / z_depth_k);
   }

   double ZValue(const Eigen::Vector2d& xy) const {
    Eigen::Vector2d vertex_i_xy = vertex_i_.head<2>();
    Eigen::Vector2d vertex_j_xy = vertex_j_.head<2>();
    Eigen::Vector2d vertex_k_xy = vertex_k_.head<2>();
    double total_weight = TriangleArea(vertex_i_xy, vertex_j_xy, vertex_k_xy);
    double weight_i = TriangleArea(xy, vertex_j_xy, vertex_k_xy) / total_weight;
    double weight_j = TriangleArea(xy, vertex_i_xy, vertex_k_xy) / total_weight;
    double weight_k = TriangleArea(xy, vertex_j_xy, vertex_i_xy) / total_weight;
    double z_depth_i = vertex_i_.w();
    double z_depth_j = vertex_j_.w();
    double z_depth_k = vertex_k_.w();
    return 1.0 / (weight_i / z_depth_i + weight_j / z_depth_j + weight_k / z_depth_k);
   }

    double AlphaValue(const Eigen::Vector2d& xy) const{

    Eigen::Vector2d vertex_i_xy = vertex_i_.head<2>();
    Eigen::Vector2d vertex_j_xy = vertex_j_.head<2>();
    Eigen::Vector2d vertex_k_xy = vertex_k_.head<2>();
    double total_weight = TriangleArea(vertex_i_xy, vertex_j_xy, vertex_k_xy);
    double weight_i = TriangleArea(xy, vertex_j_xy, vertex_k_xy) / total_weight;
    double weight_j = TriangleArea(xy, vertex_i_xy, vertex_k_xy) / total_weight;
    double weight_k = TriangleArea(xy, vertex_j_xy, vertex_i_xy) / total_weight;
    double z_depth_i = vertex_i_.w();
    double z_depth_j = vertex_j_.w();
    double z_depth_k = vertex_k_.w();
    return (alpha_i_ * weight_i / z_depth_i +  alpha_j_ * weight_j / z_depth_j + alpha_k_ * weight_k / z_depth_k) / (weight_i / z_depth_i + weight_j / z_depth_j + weight_k / z_depth_k);
    }
};

#endif //