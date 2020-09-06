#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3 &position, const glm::vec3 &color, float ambient,
                                   float diffuse, float specular, bool isShadowCaster)
    : Light(position, true, color, ambient, diffuse, specular, isShadowCaster) {}

void DirectionalLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec4(prefix + "position",
                 glm::vec4(m_position.x, m_position.y, m_position.z, m_positionW));
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
}

void DirectionalLight::setLightSpaceMatrix(const Shader &shader) const {}

void DirectionalLight::update(ERROR &errCode) {}