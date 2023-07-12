#ifndef SRC_ENGINE_H_
#define SRC_ENGINE_H_
#include "camera.h"
#include "scene.h"
#include "raster.h"
class Engine {
    public:
    Engine() {}; 
    void SetCamera(const Camera& camera);
    void SetScene(const Scene& scene);

    std::vector<char> RenderRGBFrame(size_t width, size_t height);

    private:
    Camera camera_;
    Scene scene_;
    Raster raster_;
};
#endif // SRC_ENGINE_H_