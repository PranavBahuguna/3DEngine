#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
  Light();
  Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
        const glm::vec3 &position);

  void use(const Shader &shader, ERROR &errCode) const;
  void update(ERROR &errCode){};

private:
  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  glm::vec3 m_position;
};