#include "engine.h"
#include "triangle.h"
#include "iostream"
#include <mutex>
#include <thread>
#include <mutex>

void Engine::SetCamera(const Camera& camera) {
    camera_ = camera;
}

void Engine::SetScene(const Scene &scene) {
    scene_ = scene;
}


std::vector<char> Engine::RenderRGBFrame(size_t width, size_t height) {

    // Weighted, Blended Order-Independent Transparency (OIT)

    std::vector<char> res(width * height * 3, 0);
    //std::vector<double> z_buffer(width * height, std::numeric_limits<double>::max());

    std::vector<Eigen::Vector3d> sum_alpha_multiple_color(width * height, Eigen::Vector3d::Zero());
    std::vector<double> sum_alpha(width * height, 0);
    std::vector<std::mutex> vector_mutex_of_sum(width * height);

    auto raster_functor =
        [this, width, height, &vector_mutex_of_sum, &sum_alpha,
         &sum_alpha_multiple_color](const NDCTriangle &ndc_triangle) {
          std::vector<std::pair<int, int>> mask = raster_.RasterNDCTriangleMask(ndc_triangle, width, height);
          for (auto [row, col] : mask) {
              double x = (2.0 * col - width) / width;
              double y = (2.0 * row - height) / height;
              double z_value = ndc_triangle.ZValue(Eigen::Vector2d(x, y));
              // if (z_value < z_buffer[row * width + col]) {
              // z_buffer[row * width + col] = z_value;
              Eigen::Vector3d rgb =
                  ndc_triangle.PerspectiveInterpolation(Eigen::Vector2d(x, y));
              double alpha = ndc_triangle.AlphaValue(Eigen::Vector2d(x, y));

              {
                std::lock_guard<std::mutex> lg_mutex(vector_mutex_of_sum[row * width + col]);
                sum_alpha_multiple_color[row * width + col] += alpha * rgb;
                sum_alpha[row * width + col] += alpha;
              }
                //}
          }
        };
    std::vector<std::thread> thread_pool;
    size_t total_triangle = 0;
    for (const Mesh& mesh: scene_.GetMeshs() ) {
      total_triangle += mesh.GetTriangles().size();
    }
    size_t triangle_count = 0;
    for (const Mesh& mesh: scene_.GetMeshs() ) {
        for (const Triangle& triangle : mesh.GetTriangles()) {
          NDCTriangle ndc_triangle = camera_.ProjectToNDC(triangle);
          thread_pool.emplace_back(raster_functor, ndc_triangle);
          triangle_count++;
          if (triangle_count% 16 == 0) {
            for (auto &thread : thread_pool) {
              thread.join();
            }
            thread_pool.clear();
            std::cout << "Finish : " << 100.0 * triangle_count / total_triangle << " %" << std::endl;
          }
        }
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }
    thread_pool.clear();
    std::cout << "Raster finish" << std::endl;

    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        if (sum_alpha[row * width + col] > 0) {
          Eigen::Vector3d rgb = sum_alpha_multiple_color[row * width + col] /
                                sum_alpha[row * width + col];
          char *rgb_ptr = &res[(height - row) * 3 * width + 3 * col];
          rgb_ptr[0] = rgb.x() * 255;
          rgb_ptr[1] = rgb.y() * 255;
          rgb_ptr[2] = rgb.z() * 255;
        }
      }
    }
    return res;

}
