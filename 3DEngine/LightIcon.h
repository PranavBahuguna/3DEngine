#pragma once

#include "Drawable.h"
#include "Light.h"
#include "Resources.h"
#include "Texture.h"

#include <memory>

#include <glm/glm.hpp>

class LightIcon : public Drawable {
public:
  LightIcon(const std::string &lightName);

  void draw(ERROR &errCode, const Shader &shader) const override;

private:
  TexSptr m_texture;
  LightSptr m_light;

  std::vector<std::vector<GLfloat>> m_vertices;

  GLuint m_VAO;
  GLuint m_VBO;
};