#include "PointLight.h"

PointLight::PointLight(const glm::vec3 &position, const glm::vec3 &color, float ambient,
                       float diffuse, float specular, float constant, float linear, float quadratic,
                       bool isShadowCaster)
    : Light(position, false, color, ambient, diffuse, specular, isShadowCaster, constant, linear,
            quadratic) {}

void PointLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec4(prefix + "position",
                 glm::vec4(m_position.x, m_position.y, m_position.z, m_positionW));
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
  shader.setFloat(prefix + "constant", m_constant);
  shader.setFloat(prefix + "linear", m_linear);
  shader.setFloat(prefix + "quadratic", m_quadratic);
}

void PointLight::setLightSpaceMatrix(const Shader &shader) const {}

void PointLight::update(ERROR &errCode) {}