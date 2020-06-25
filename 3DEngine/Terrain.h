#pragma once

#include "Model.h"

class Terrain : public Model {
public:
  Terrain(const std::string &name, const glm::uvec2 &nTiles, const glm::vec2 &tileDimensions,
          const glm::vec2 &tileTexMapping = {1.0f, 1.0f});

  void load(ERROR &errCode) override;

private:
  void generateMesh();

  glm::uvec2 m_nTiles;
  glm::vec2 m_tileDimensions;
  glm::vec2 m_tileTexMapping;
};