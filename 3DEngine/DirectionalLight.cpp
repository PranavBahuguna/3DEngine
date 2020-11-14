#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Transform &transform, const glm::vec3 &color,
                                   const Phong &phong, bool isShadowCaster)
    : Light(transform, color, phong, isShadowCaster) {}

void DirectionalLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec3(prefix + "position", m_transform.getFront());
  shader.setBool(prefix + "isDir", true);
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambient", m_phong.ambient);
  shader.setFloat(prefix + "diffuse", m_phong.diffuse);
  shader.setFloat(prefix + "specular", m_phong.specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
}

void DirectionalLight::setLightSpaceMatrix(const Shader &shader) const {}

void DirectionalLight::update(ERROR &errCode) {}