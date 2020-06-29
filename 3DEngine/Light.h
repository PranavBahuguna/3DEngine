#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
  Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

  virtual void use(const Shader &shader, size_t index, ERROR &errCode) const;
  virtual void update(ERROR &errCode){};

protected:
  mutable std::string m_paramPrefix;

private:
  void setParamPrefix(size_t index) const;

  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
};