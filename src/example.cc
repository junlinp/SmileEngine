#include "libgltf/libgltf.h"
#include "triangle.h"
#include "scene.h"
#include "camera.h"
#include "engine.h"
#include "iostream"
#include "mesh.h"
#include "bitmap_image.hpp"

//#include "gltf_loader/gltf_loader.h"

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
    for (int i = 0; i < 1024; i++) {
    	scene.PushMesh(mesh);
    }


    std::cout << "load" << std::endl;
    std::vector<Mesh> meshs;

    //LoadGLTF("./eva-01_rig.gltf", &meshs);
    //std::cout << "load finish" << std::endl;

    //    for(auto mesh : meshs) {
        //std::cout << "triangles : " << mesh.GetTriangles().size() << std::endl;
        //scene.PushMesh(mesh);
	//}

    Engine engine;
    engine.SetCamera(camera);
    engine.SetScene(scene);




    std::cout << "Scene " << scene.GetMeshs().size() << " Meshs exists" << std::endl;
    size_t height = 1080;
    size_t width = 1920;
    std::vector<char> rgb = engine.RenderRGBFrame(width, height);

    std::cout << "Raster Finish" << std::endl;
    
    bitmap_image image(width, height);
    for (int row = 0; row < height; row++) {
    	for (int col = 0; col < width;col++) {
		char* color_ptr = rgb.data() + row * 3 * width + 3 * col;
		image.set_pixel(col, row, color_ptr[0], color_ptr[1], color_ptr[2]);
	}
    }		
    image.save_image("test.bmp");    
    return 0;
}
