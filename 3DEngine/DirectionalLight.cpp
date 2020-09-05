#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3 &position, const glm::vec3 &color, float ambient,
                                   float diffuse, float specular, bool isShadowCaster)
    : Light(position, color, ambient, diffuse, specular, isShadowCaster) {

  // w-value of 0 indicates to shader that this is a direction
  m_position.w = 0.0f;
}

void DirectionalLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec4(prefix + "position", m_position);
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
}