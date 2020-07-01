#include "Light.h"

#include <sstream>

// Constructor
Light::Light(LightType type, const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, const glm::vec3 &position, GLfloat constant, GLfloat linear,
             GLfloat quadratic, const glm::vec3 &coneDir, GLfloat innerConeAngle,
             GLfloat outerConeAngle)
    : m_type(type), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
      m_constant(constant), m_linear(linear), m_quadratic(quadratic), m_coneDir(coneDir),
      m_innerConeAngle(0.0f), m_outerConeAngle(0.0f) {

  // w-value of 0 indicates to shader that this is a direction, w-value of 1 indicates that this is
  // a position
  GLfloat positionW = (type == LightType::DIRECTIONAL_LIGHT) ? 0.0f : 1.0f;
  m_position = glm::vec4(position.x, position.y, position.z, positionW);

  // Set cone angles only if we are using a spot light
  if (type == LightType::SPOT_LIGHT) {
    m_innerConeAngle = glm::cos(glm::radians(innerConeAngle));
    m_outerConeAngle = glm::cos(glm::radians(outerConeAngle));
  }
}

// Supply shader with light data
void Light::use(const Shader &shader, size_t index, ERROR &errCode) const {
  // Get the shader uniform name prefix
  std::ostringstream ss;
  ss << "lights[" << index << "].";
  const std::string prefix = ss.str();

  shader.setVec3(prefix + "ambient", m_ambient, errCode);
  shader.setVec3(prefix + "diffuse", m_diffuse, errCode);
  shader.setVec3(prefix + "specular", m_specular, errCode);
  shader.setVec4(prefix + "position", m_position, errCode);

  if (m_type == LightType::POINT_LIGHT || m_type == LightType::SPOT_LIGHT) {
    shader.setFloat(prefix + "constant", m_constant, errCode);
    shader.setFloat(prefix + "linear", m_linear, errCode);
    shader.setFloat(prefix + "quadratic", m_quadratic, errCode);

    if (m_type == LightType::SPOT_LIGHT) {
      shader.setVec3(prefix + "coneDir", m_coneDir, errCode);
      shader.setFloat(prefix + "innerConeAngle", m_innerConeAngle, errCode);
      shader.setFloat(prefix + "outerConeAngle", m_outerConeAngle, errCode);
    }
  }
}