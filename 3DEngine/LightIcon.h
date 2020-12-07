#pragma once

#include "DirectionalLight.h"
#include "Component.h"
#include "Light.h"
#include "PointLight.h"
#include "ResourceManager.h"
#include "SpotLight.h"
#include "Texture.h"

#include <memory>

#include <glm/glm.hpp>

class LightIcon : public Component {
public:
  LightIcon(LightSptr light);
  LightIcon(const std::shared_ptr<GameObject> &owner, LightSptr light);

  void draw(ERROR &errCode, const Shader &shader) override;

private:
  TexSptr m_texture;
  glm::vec4 m_lightColor;

  std::vector<std::vector<GLfloat>> m_vertices;

  GLuint m_VAO;
  GLuint m_VBO;
};