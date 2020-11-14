#include "PointLight.h"

PointLight::PointLight(const Transform &transform, const glm::vec3 &color, const Phong &phong,
                       const Attenuation &attenuation, bool isShadowCaster)
    : Light(transform, color, phong, isShadowCaster, attenuation) {}

void PointLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec3(prefix + "position", m_transform.getPosition());
  shader.setBool(prefix + "isDir", false);
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambient", m_phong.ambient);
  shader.setFloat(prefix + "diffuse", m_phong.diffuse);
  shader.setFloat(prefix + "specular", m_phong.specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
  shader.setFloat(prefix + "constant", m_attenuation.constant);
  shader.setFloat(prefix + "linear", m_attenuation.linear);
  shader.setFloat(prefix + "quadratic", m_attenuation.quadratic);
}

void PointLight::setLightSpaceMatrix(const Shader &shader) const {}

void PointLight::update(ERROR &errCode) {}