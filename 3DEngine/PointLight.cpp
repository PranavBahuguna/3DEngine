#include "PointLight.h"

// Constructor
PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const glm::vec3 &position, GLfloat constant,
                       GLfloat linear, GLfloat quadratic)
    : Light(ambient, diffuse, specular), m_position(position), m_constant(constant),
      m_linear(linear), m_quadratic(quadratic) {}

// Supply shader with point light data
void PointLight::use(const Shader &shader, ERROR &errCode) const {
  Light::use(shader, errCode);

  GLuint positionId = shader.getParamId("light.position", errCode);
  GLuint constantId = shader.getParamId("light.constant", errCode);
  GLuint linearId = shader.getParamId("light.linear", errCode);
  GLuint quadraticId = shader.getParamId("light.quadratic", errCode);

  if (errCode != ERROR_OK)
    return;

  // w-value of 1 indicates to shader that this is a position
  glUniform4f(positionId, m_position.x, m_position.y, m_position.z, 1.0f);
  glUniform1f(constantId, m_constant);
  glUniform1f(linearId, m_linear);
  glUniform1f(quadraticId, m_quadratic);
}