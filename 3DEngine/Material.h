#pragma once

#include "Shader.h"

class Material {
public:
  Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
           GLfloat shininess);

  void use(const Shader &shader) const;

private:
  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  GLfloat m_shininess;
};