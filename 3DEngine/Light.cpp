#include "Light.h"

// Constructor - full white light by default
Light::Light() : Light(glm::vec3(1.0f), glm::vec3(0.0f), 1.0f, 1.0f) {}

// Constructor - main
Light::Light(const glm::vec3 &rgb, const glm::vec3 &direction, GLfloat ambientIntensity,
             GLfloat diffuseIntensity)
    : m_color(rgb), m_direction(direction), m_ambientIntensity(ambientIntensity),
      m_diffuseIntensity(diffuseIntensity) {}

// Apply light to the scene
void Light::use(const Shader &shader) const {
  glUniform3f(shader.getColorId(), m_color.x, m_color.y, m_color.z);
  glUniform3f(shader.getLightDirectionId(), m_direction.x, m_direction.y, m_direction.z);
  glUniform1f(shader.getAmbientIntensityId(), m_ambientIntensity);
  glUniform1f(shader.getDiffuseIntensityId(), m_diffuseIntensity);
}