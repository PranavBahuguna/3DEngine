#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &color, float ambient,
                     float diffuse, float specular, float constant, float linear, float quadratic,
                     const glm::vec3 &coneDir, float innerConeAngle, float outerConeAngle,
                     bool isShadowCaster)
    : Light(position, color, ambient, diffuse, specular, isShadowCaster, constant, linear,
            quadratic, coneDir, innerConeAngle, outerConeAngle) {}

void SpotLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec4(prefix + "position", m_position);
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
  shader.setFloat(prefix + "constant", m_constant);
  shader.setFloat(prefix + "linear", m_linear);
  shader.setFloat(prefix + "quadratic", m_quadratic);
  shader.setVec3(prefix + "coneDir", m_coneDir);
  shader.setFloat(prefix + "innerConeAngle", m_innerConeAngle);
  shader.setFloat(prefix + "outerConeAngle", m_outerConeAngle);
}