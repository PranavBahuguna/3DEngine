#pragma once

#include "DirectionalLight.h"
#include "Drawable.h"
#include "Light.h"
#include "PointLight.h"
#include "ResourceManager.h"
#include "SpotLight.h"
#include "Texture.h"

#include <memory>

#include <glm/glm.hpp>

class LightIcon : public Drawable {
public:
  //LightIcon(Light &light);
  LightIcon(LightSptr light);

  void draw(ERROR &errCode, const Shader &shader) const override;

private:
  void init(Light *light, const glm::vec2 &texIndices);

  TexSptr m_texture;
  glm::vec4 m_lightColor;

  std::vector<std::vector<GLfloat>> m_vertices;

  GLuint m_VAO;
  GLuint m_VBO;
};