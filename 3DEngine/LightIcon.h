#pragma once

#include "Resources.h"

#include <memory>

#include <glm/glm.hpp>

class LightIcon {
public:
  LightIcon(const std::string &lightName);
  ~LightIcon() {}

  void draw(const Shader &shader, ERROR &errCode);

private:
  TexPtr m_texture;
  LightPtr m_light;

  glm::vec3 m_pos;

  //std::vector<std::vector<GLfloat>> m_vertices;
  std::vector<std::vector<GLfloat>> m_vertices;

  GLuint m_VAO;
  GLuint m_VBO;
};