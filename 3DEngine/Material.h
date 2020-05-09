#pragma once

#include "Shader.h"

#include <GL/glew.h>

class Material {
public:
  Material();
  Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
           GLfloat m_shininess);

  void use(const Shader &shader) const;

private:
  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  GLfloat m_shininess;
};