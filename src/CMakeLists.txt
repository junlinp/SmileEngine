cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(engine triangle.cc camera.cc scene.cc engine.cc raster.cc)
target_link_libraries(engine Eigen3::Eigen)

add_subdirectory(gltf_loader)

add_executable(example example.cc)
target_include_directories(example PUBLIC ${OpenCV_INCLUDE_DIR})
target_link_libraries(example PUBLIC engine ${OpenCV_LIBS} gltf_loader)