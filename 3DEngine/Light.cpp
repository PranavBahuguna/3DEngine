#include "Light.h"

#include <stdexcept>

// Constructor
Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

// Supply shader with light data
void Light::use(const Shader &shader, ERROR &errCode) const {
  shader.setVec3("light.ambient", m_ambient, errCode);
  shader.setVec3("light.diffuse", m_diffuse, errCode);
  shader.setVec3("light.specular", m_specular, errCode);
}