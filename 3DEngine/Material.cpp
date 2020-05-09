#include "Material.h"

// Constructor - set default values
Material::Material() : Material(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), 32.0f) {}

// Constructor - main
Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                   GLfloat shininess)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}

// Use this material
void Material::use(const Shader &shader) const {
  const auto &materialIds = shader.getMaterialIds();
  glUniform3f(materialIds[0], m_ambient.x, m_ambient.y, m_ambient.z);
  glUniform3f(materialIds[1], m_diffuse.x, m_diffuse.y, m_diffuse.z);
  glUniform3f(materialIds[2], m_specular.x, m_specular.y, m_specular.z);
  glUniform1f(materialIds[3], m_shininess);
}
