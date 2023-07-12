#ifndef SRC_MESH_H_
#define SRC_MESH_H_
#include "triangle.h"
#include <type_traits>
#include <vector>
#include "Eigen/Dense"

class Mesh {
    public:
      Mesh() {};

    template<class T>
    void SetIndices(const std::vector<T>& indices) {
        static_assert(std::is_integral_v<T>);
        assert(!indices.empty()); 
        for(T v : indices) {
            indices_.push_back(static_cast<size_t>(v));
        }
    }

    void SetVertices(const std::vector<Eigen::Vector3f> &vertices) {
      vertices_ = vertices;
    }

    std::vector<Triangle> GetTriangles() const {
        std::vector<Triangle> tri_angles;
        assert(indices_.size() % 3 == 0);

        for(int i = 0; i < indices_.size() / 3; i++) {
            Triangle angle(
                vertices_[indices_[3 * i]].cast<double>(),
                vertices_[indices_[3 * i + 1]].cast<double>(),
                vertices_[indices_[3 * i + 2]].cast<double>()
            );

            tri_angles.push_back(angle);
        }
        return tri_angles;
    }

    private:
      std::vector<size_t> indices_;
      std::vector<Eigen::Vector3f> vertices_;

};
#endif // SRC_MESH_H_