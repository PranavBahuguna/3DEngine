#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
  Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

  virtual void use(const Shader &shader, ERROR &errCode) const;
  virtual void update(ERROR &errCode){};

private:
  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
};