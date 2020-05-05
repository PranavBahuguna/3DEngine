#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
  Light();
  Light(const glm::vec3 &rgb, const glm::vec3 &direction, GLfloat ambientIntensity,
        GLfloat diffuseIntensity);

  void use(const Shader &shader) const;
  void update(ERROR &errCode){};

protected:
  glm::vec3 m_color;
  glm::vec3 m_pos;

  GLfloat m_ambientIntensity;
  GLfloat m_diffuseIntensity;
};