#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_
#include "Eigen/Dense"
#include "triangle.h"
class Camera {
private:
  Eigen::Vector4d ToCameraCooordinate(const Eigen::Vector3d &point) const;

public:
  Camera(
      const Eigen::Quaternion<double> rotation = Eigen::Quaterniond::Identity(),
      const Eigen::Vector3d &translation = Eigen::Vector3d::Zero(),
      double near = 0.1,
      double far = 1000.0,
      double aspect = 16.0 / 9.0,
      double fov = 3.1415926 / 4
      );
  NDCTriangle ProjectToNDC(const Triangle &triangle);

  Eigen::Quaternion<double> rotation_;
  Eigen::Vector3d translation_;

  double near_;
  double far_;
  double aspect_;
  double fov_;
};
#endif // SRC_CAMERA_H_