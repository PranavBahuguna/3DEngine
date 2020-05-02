#include "Light.h"

// Constructor - full white light by default
Light::Light() : Light(glm::vec3(1.0f), 1.0f) {}

// Constructor - main
Light::Light(const glm::vec3 &rgb, GLfloat intensity) : m_color(rgb), m_intensity(intensity) {}

// Apply ambient light to the scene
void Light::use(const Shader &shader) const {
  glUniform3f(shader.getColorId(), m_color.x, m_color.y, m_color.z);
  glUniform1f(shader.getAmbientIntensityId(), m_intensity);
}