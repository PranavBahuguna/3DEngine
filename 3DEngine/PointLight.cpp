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

  // w-value of 1 indicates to shader that this is a position
  shader.setVec4("light.position", m_position.x, m_position.y, m_position.z, 1.0f, errCode);
  shader.setFloat("light.constant", m_constant, errCode);
  shader.setFloat("light.linear", m_linear, errCode);
  shader.setFloat("light.quadratic", m_quadratic, errCode);
}