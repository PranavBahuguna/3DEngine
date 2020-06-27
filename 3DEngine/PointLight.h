#pragma once

#include "Light.h"

class PointLight : public Light {
public:
  PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
             const glm::vec3 &position, GLfloat constant, GLfloat linear, GLfloat quadratic);

  void use(const Shader &shader, ERROR &errCode) const override;

private:
  glm::vec3 m_position;
  GLfloat m_constant;
  GLfloat m_linear;
  GLfloat m_quadratic;
};