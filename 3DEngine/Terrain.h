#pragma once

#include "Model.h"

class Terrain : public Model {
public:
  Terrain(const std::string &name, const glm::uvec2 &nTiles, const GLfloat tileSize);

private:
  void loadAssets(const aiScene &scene, ERROR &errCode) override;
  void generateMesh(ERROR &errCode);

  glm::uvec2 m_nTiles;
  GLfloat m_tileSize;
};