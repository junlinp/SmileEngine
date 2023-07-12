#include "camera.h"
#include "triangle.h"

Camera::Camera(const Eigen::Quaternion<double> rotation,
               const Eigen::Vector3d &translation, double near, double far,
               double aspect, double fov)
    : rotation_(rotation), translation_(translation), near_(near), far_(far),
      aspect_(aspect), fov_(fov) {}

Eigen::Vector4d Camera::ToCameraCooordinate(const Eigen::Vector3d& point) const{
    Eigen::Vector4d camera_coordinate =  (rotation_.toRotationMatrix() * point + translation_).homogeneous();
    Eigen::Matrix4d project_matrix;
    project_matrix.setZero();

    project_matrix(0, 0) = 2.0 * near_ * std::tan(0.5 * fov_) / aspect_;
    project_matrix(1, 1) = 2.0 * near_ * std::tan(0.5 * fov_);

    project_matrix(2, 2) = - (near_ + far_) / (far_ - near_);
    project_matrix(2, 3) = - 2.0 * near_ * far_ / (far_ - near_);

    project_matrix(3, 2) = -1.0;

    return project_matrix * camera_coordinate;
}

NDCTriangle Camera::ProjectToNDC(const Triangle &triangle) {
  return NDCTriangle(ToCameraCooordinate(triangle.i()),
                     ToCameraCooordinate(triangle.j()),
                     ToCameraCooordinate(triangle.k()));
}

