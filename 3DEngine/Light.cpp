#include "Light.h"

#include <sstream>

// Constructor
Light::Light(LightType type, const glm::vec3 &position, const glm::vec3 &color, float ambient,
             float diffuse, float specular, float constant, float linear, float quadratic,
             const glm::vec3 &coneDir, float innerConeAngle, float outerConeAngle)
    : m_type(type), m_color(color), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
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
void Light::use(const Shader &shader, size_t index) const {
  // Get the shader uniform name prefix
  std::ostringstream ss;
  ss << "lights[" << index << "].";
  const std::string prefix = ss.str();

  shader.setVec4(prefix + "position", m_position);
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);

  if (m_type == LightType::POINT_LIGHT || m_type == LightType::SPOT_LIGHT) {
    shader.setFloat(prefix + "constant", m_constant);
    shader.setFloat(prefix + "linear", m_linear);
    shader.setFloat(prefix + "quadratic", m_quadratic);

    if (m_type == LightType::SPOT_LIGHT) {
      shader.setVec3(prefix + "coneDir", m_coneDir);
      shader.setFloat(prefix + "innerConeAngle", m_innerConeAngle);
      shader.setFloat(prefix + "outerConeAngle", m_outerConeAngle);
    }
  }
}