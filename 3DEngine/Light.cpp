#include "Light.h"

#include <sstream>

Light::Light(const glm::vec3 &position, const glm::vec3 &color, float ambient, float diffuse,
             float specular, bool isShadowCaster, float constant, float linear, float quadratic,
             const glm::vec3 &coneDir, float innerConeAngle, float outerConeAngle)
    : m_color(color), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
      m_isShadowCaster(isShadowCaster), m_constant(constant), m_linear(linear),
      m_quadratic(quadratic), m_coneDir(coneDir) {

  // By default, w-value of 1 indicates to shader that this is a position
  m_position = glm::vec4(position.x, position.y, position.z, 1.0f);

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