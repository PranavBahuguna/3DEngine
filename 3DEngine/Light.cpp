#include "Light.h"

#include <stdexcept>

// Constructor
Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

// Supply shader with light data
void Light::use(const Shader &shader, ERROR &errCode) const {
  glUniform3fv(shader.getParamId("light.ambient", errCode), 1, glm::value_ptr(m_ambient));
  glUniform3fv(shader.getParamId("light.diffuse", errCode), 1, glm::value_ptr(m_diffuse));
  glUniform3fv(shader.getParamId("light.specular", errCode), 1, glm::value_ptr(m_specular));
}