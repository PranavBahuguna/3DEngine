#pragma once

#include "CubeMap.h"
#include "Drawable.h"

class Skybox : public Drawable {
public:
  Skybox(const std::string &cubeMapFolder, const std::string &extension);

  void draw(ERROR &errCode, const Shader &shader) const;

private:
  CbmSptr m_cubeMap;

  GLuint m_VAO;
  GLuint m_VBO;
};
