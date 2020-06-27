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

  GLuint coneDirId = shader.getParamId("light.coneDir", errCode);
  GLuint innerConeAngleId = shader.getParamId("light.innerConeAngle", errCode);
  GLuint outerConeAngleId = shader.getParamId("light.outerConeAngle", errCode);

  if (errCode != ERROR_OK)
    return;

  glUniform3f(coneDirId, m_coneDir.x, m_coneDir.y, m_coneDir.z);
  glUniform1f(innerConeAngleId, m_innerConeAngle);
  glUniform1f(outerConeAngleId, m_outerConeAngle);
}