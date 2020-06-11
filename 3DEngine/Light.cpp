#include "Light.h"

#include <stdexcept>

// Constructor - full white light at origin by default
Light::Light() : Light(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f)) {}

// Constructor - main
Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
             const glm::vec3 &position)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_position(position) {}

// Apply light to the scene
void Light::use(const Shader &shader, ERROR &errCode) const {
  GLuint ambientId = shader.getParamId("light.ambient", errCode);
  GLuint diffuseId = shader.getParamId("light.diffuse", errCode);
  GLuint specularId = shader.getParamId("light.specular", errCode);
  GLuint positionId = shader.getParamId("light.position", errCode);

  if (errCode != ERROR_OK)
    return;

  glUniform3f(ambientId, m_ambient.x, m_ambient.y, m_ambient.z);
  glUniform3f(diffuseId, m_diffuse.x, m_diffuse.y, m_diffuse.z);
  glUniform3f(specularId, m_specular.x, m_specular.y, m_specular.z);
  glUniform3f(positionId, m_position.x, m_position.y, m_position.z);
}