#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
  Light();
  Light(const glm::vec3 &rgb, GLfloat intensity);

  void use(const Shader &shader) const;

private:
  glm::vec3 m_color;
  GLfloat m_intensity;
};