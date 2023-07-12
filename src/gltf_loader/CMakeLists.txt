cmake_minimum_required(VERSION 3.20)

find_package(Eigen3 REQUIRED)

add_library(gltf_loader gltf_loader.cc)
target_include_directories(gltf_loader PUBLIC "/usr/local/include" ${CMAKE_SOURCE_DIR})
target_link_directories(gltf_loader PUBLIC "/usr/local/lib")
target_link_libraries(gltf_loader libgltf Eigen3::Eigen)