#include "SpotLight.h"

// Constructor
SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &position, const glm::vec3 &coneDir, GLfloat innerConeAngle,
                     GLfloat outerConeAngle, GLfloat constant, GLfloat linear, GLfloat quadratic)
    : PointLight(ambient, diffuse, specular, position, constant, linear, quadratic),
      m_coneDir(coneDir), m_innerConeAngle(glm::cos(glm::radians(innerConeAngle))),
      m_outerConeAngle(glm::cos(glm::radians(outerConeAngle))) {}

// Supply shader with spot light data
void SpotLight::use(const Shader &shader, ERROR &errCode) const {
  PointLight::use(shader, errCode);

  shader.setVec3("light.coneDir", m_coneDir, errCode);
  shader.setFloat("light.innerConeAngle", m_innerConeAngle, errCode);
  shader.setFloat("light.outerConeAngle", m_outerConeAngle, errCode);
}