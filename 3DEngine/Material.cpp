#include "Material.h"

// Constructor
Material::Material(const aiMaterial &mat) {
  mat.Get(AI_MATKEY_COLOR_AMBIENT, m_ambient);
  mat.Get(AI_MATKEY_COLOR_DIFFUSE, m_diffuse);
  mat.Get(AI_MATKEY_COLOR_SPECULAR, m_specular);
  mat.Get(AI_MATKEY_SHININESS, m_shininess);
}

// Use this material
void Material::use(const Shader &shader) const {
  const auto &materialIds = shader.getMaterialIds();
  glUniform3f(materialIds[0], m_ambient.r, m_ambient.g, m_ambient.b);
  glUniform3f(materialIds[1], m_diffuse.r, m_diffuse.g, m_diffuse.b);
  glUniform3f(materialIds[2], m_specular.r, m_specular.g, m_specular.b);
  glUniform1f(materialIds[3], m_shininess);
}
