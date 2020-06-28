#include "Material.h"

#include <stdexcept>

// Constructor
Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                   GLfloat shininess)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}

// Use this material
void Material::use(const Shader &shader, ERROR &errCode) const {
  glUniform3fv(shader.getParamId("material.ambient", errCode), 1, glm::value_ptr(m_ambient));
  glUniform3fv(shader.getParamId("material.diffuse", errCode), 1, glm::value_ptr(m_diffuse));
  glUniform3fv(shader.getParamId("material.specular", errCode), 1, glm::value_ptr(m_specular));
  glUniform1f(shader.getParamId("material.shininess", errCode), m_shininess);
}
