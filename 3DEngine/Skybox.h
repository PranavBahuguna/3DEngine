#pragma once

#include "Camera.h"
#include "Component.h"
#include "CubeMap.h"

class Skybox : public Component {
public:
  Skybox(const std::string &cubeMapFolder, const std::vector<std::string> &files);
  Skybox(std::shared_ptr<GameObject> owner, const std::string &cubeMapFolder,
         const std::vector<std::string> &files);

  void draw(ERROR &errCode, const Shader &shader) override;

private:
  CbmSptr m_cubeMap;

  GLuint m_VAO;
  GLuint m_VBO;
};
