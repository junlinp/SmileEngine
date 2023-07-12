#include "libgltf/libgltf.h"
#include "triangle.h"
#include "scene.h"
#include "camera.h"
#include "engine.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "iostream"
#include "mesh.h"

#include "gltf_loader/gltf_loader.h"

int main(int argc, char** argv) {
    Camera camera(Eigen::Quaternion<double>::Identity(), Eigen::Vector3d::Zero());
    std::vector<size_t> indices = {0, 1, 2, 3, 2 ,1};
    std::vector<Eigen::Vector3f> vertices = {
        Eigen::Vector3f(-1.0, 1.0, -0.5), Eigen::Vector3f(-1.0, -1.0, -0.5),
        Eigen::Vector3f(1.0, 1.0, -0.5), Eigen::Vector3f(1.0, -1.0, -0.5)};

    Mesh mesh;
    mesh.SetIndices(indices);
    mesh.SetVertices(vertices);

    Scene scene; 
    scene.PushMesh(mesh);


    std::cout << "load" << std::endl;
    std::vector<Mesh> meshs;

    LoadGLTF("./eva-01_rig.gltf", &meshs);
    std::cout << "load finish" << std::endl;

    for(auto mesh : meshs) {
        //std::cout << "triangles : " << mesh.GetTriangles().size() << std::endl;
        scene.PushMesh(mesh);
    }

    Engine engine;
    engine.SetCamera(camera);
    engine.SetScene(scene);




    std::cout << "Scene " << scene.GetMeshs().size() << " Meshs exists" << std::endl;
    size_t height = 480;
    size_t width = 640;
    std::vector<char> rgb = engine.RenderRGBFrame(width, height);

    cv::Mat M(height, width, CV_8UC3, cv::Scalar(0, 0, 255));
    std::copy(rgb.begin(), rgb.end(), M.ptr(0));

    cv::cvtColor(M,M, cv::COLOR_BGR2RGB);
    cv::imwrite("test.png", M);
    return 0;
}