#include "Material.h"

// Constructor - default values to zero
Material::Material() : m_specularIntensity(0.0f), m_shine(0.0f) {}

// Constructor - main
Material::Material(GLfloat sIntensity, GLfloat shine)
    : m_specularIntensity(sIntensity), m_shine(shine) {}

// Use this material
void Material::use(const Shader &shader) const { 
  glUniform1f(shader.getSpecularIntensityId(), m_specularIntensity);
  glUniform1f(shader.getShineId(), m_shine);
}
