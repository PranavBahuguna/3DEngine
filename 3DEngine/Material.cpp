#include "Material.h"

// Constructor - set default values
Material::Material(const aiMaterial *mat) {
  aiColor3D ambient;
  aiColor3D diffuse;
  aiColor3D specular;
  float shininess;

  mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
  mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
  mat->Get(AI_MATKEY_SHININESS, m_shininess);

  m_ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
  m_diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
  m_specular = glm::vec3(specular.r, specular.g, specular.b);
}

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
