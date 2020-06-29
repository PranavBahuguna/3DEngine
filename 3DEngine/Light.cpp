#include "Light.h"

#include <sstream>
#include <stdexcept>

// Constructor
Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

// Supply shader with light data
void Light::use(const Shader &shader, size_t index, ERROR &errCode) const {
  setParamPrefix(index);

  shader.setVec3(m_paramPrefix + ".ambient", m_ambient, errCode);
  shader.setVec3(m_paramPrefix + ".diffuse", m_diffuse, errCode);
  shader.setVec3(m_paramPrefix + ".specular", m_specular, errCode);
}

// Sets the shader uniform name prefix
void Light::setParamPrefix(size_t index) const {
  std::ostringstream ss;
  ss << "lights[" << index << "]";
  m_paramPrefix = ss.str();
}