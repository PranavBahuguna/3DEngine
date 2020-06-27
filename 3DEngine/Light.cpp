#include "Light.h"

#include <stdexcept>

// Constructor
Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

// Supply shader with light data
void Light::use(const Shader &shader, ERROR &errCode) const {
  GLuint ambientId = shader.getParamId("light.ambient", errCode);
  GLuint diffuseId = shader.getParamId("light.diffuse", errCode);
  GLuint specularId = shader.getParamId("light.specular", errCode);

  if (errCode != ERROR_OK)
    return;

  glUniform3f(ambientId, m_ambient.x, m_ambient.y, m_ambient.z);
  glUniform3f(diffuseId, m_diffuse.x, m_diffuse.y, m_diffuse.z);
  glUniform3f(specularId, m_specular.x, m_specular.y, m_specular.z);
}