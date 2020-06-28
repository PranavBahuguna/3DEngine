#include "Material.h"

#include <stdexcept>

// Constructor
Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                   GLfloat shininess)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}

// Use this material
void Material::use(const Shader &shader, ERROR &errCode) const {
  shader.setVec3("material.ambient", m_ambient, errCode);
  shader.setVec3("material.diffuse", m_diffuse, errCode);
  shader.setVec3("material.specular", m_specular, errCode);
  shader.setFloat("material.shininess", m_shininess, errCode);
}
