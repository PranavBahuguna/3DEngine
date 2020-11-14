#include "SpotLight.h"
#include "Projection.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

SpotLight::SpotLight(const Transform &transform, const glm::vec3 &color, const Phong &phong,
                     const Attenuation &attenuation, float innerConeAngle, float outerConeAngle,
                     bool isShadowCaster)
    : Light(transform, color, phong, isShadowCaster, attenuation, innerConeAngle, outerConeAngle),
      m_projection(innerConeAngle * 2.0f, 1.0f, 1.0f, 20.0f) {}

void SpotLight::use(const Shader &shader, const std::string &prefix) const {
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
  shader.setVec3(prefix + "coneDir", m_transform.getFront());
  shader.setFloat(prefix + "innerConeAngle", m_innerConeAngle);
  shader.setFloat(prefix + "outerConeAngle", m_outerConeAngle);
}

void SpotLight::setLightSpaceMatrix(const Shader &shader) const {
  shader.setMat4("lightSpaceMatrix", m_projection.getProjection() * m_transform.getView());
}

void SpotLight::update(ERROR &errCode) {}