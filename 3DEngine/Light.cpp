#include "Light.h"

#include <sstream>

Light::Light(const glm::vec3 &position, bool isDir, const glm::vec3 &color, float ambient,
             float diffuse, float specular, bool isShadowCaster, float constant, float linear,
             float quadratic, const glm::vec3 &coneDir, float innerConeAngle, float outerConeAngle)
    : m_position(position), m_color(color), m_ambient(ambient), m_diffuse(diffuse),
      m_specular(specular), m_isShadowCaster(isShadowCaster), m_constant(constant),
      m_linear(linear), m_quadratic(quadratic), m_coneDir(coneDir) {

  // The w-value indicates to shader whether the vector provided represents a position (w = 1) or
  // a direction (w = 0)
  m_positionW = isDir ? 0.0f : 1.0f;

  m_innerConeAngle = glm::cos(glm::radians(innerConeAngle));
  m_outerConeAngle = glm::cos(glm::radians(outerConeAngle));
}

// Use accessor method
void Light::use(const Shader &shader, size_t index) const {
  // Get the shader uniform name prefix, then call use method
  std::ostringstream ss;
  ss << "lights[" << index << "].";

  use(shader, ss.str());
}