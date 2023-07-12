#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

#include "mesh.h"

class Scene {
public:
  void PushMesh(const Mesh &mesh) { meshs_.push_back(mesh); }
  std::vector<Mesh> GetMeshs() const { return meshs_; }
  std::vector<Mesh> meshs_;
};

#endif  // SRC_SCENE_H_