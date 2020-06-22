#pragma once

#include "Model.h"

class Terrain : public Model {
public:
  Terrain(const std::string &name, const glm::uvec2 &nTiles, const GLfloat tileSize);

  void load(ERROR &errCode) override;

private:
  void generateMesh();

  glm::uvec2 m_nTiles;
  GLfloat m_tileSize;
};