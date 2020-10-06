#pragma once

#include "CubeMap.h"
#include "Drawable.h"
#include "Camera.h"

class Skybox : public Drawable {
public:
  Skybox(const std::string &cubeMapFolder, const std::vector<std::string> &files);

  void draw(ERROR &errCode, const Shader &shader) const override;

private:
  CbmSptr m_cubeMap;
  std::shared_ptr<Camera> m_camera;

  GLuint m_VAO;
  GLuint m_VBO;
};
