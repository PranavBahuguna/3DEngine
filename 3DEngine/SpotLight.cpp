#include "SpotLight.h"

// Constructor
SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &position, const glm::vec3 &coneDir, GLfloat innerConeAngle,
                     GLfloat outerConeAngle, GLfloat constant, GLfloat linear, GLfloat quadratic)
    : PointLight(ambient, diffuse, specular, position, constant, linear, quadratic),
      m_coneDir(coneDir), m_innerConeAngle(glm::cos(glm::radians(innerConeAngle))),
      m_outerConeAngle(glm::cos(glm::radians(outerConeAngle))) {}

// Supply shader with spot light data
void SpotLight::use(const Shader &shader, size_t index, ERROR &errCode) const {
  PointLight::use(shader, index, errCode);

  shader.setVec3(m_paramPrefix + ".coneDir", m_coneDir, errCode);
  shader.setFloat(m_paramPrefix + ".innerConeAngle", m_innerConeAngle, errCode);
  shader.setFloat(m_paramPrefix + ".outerConeAngle", m_outerConeAngle, errCode);
}