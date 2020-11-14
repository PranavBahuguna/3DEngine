#include "Light.h"

#include <sstream>

Light::Light(const Transform &transform, const glm::vec3 &color, const Phong &phong,
             bool isShadowCaster, const Attenuation &attenuation, float innerConeAngle,
             float outerConeAngle)
    : m_transform(transform), m_color(color), m_phong(phong), m_isShadowCaster(isShadowCaster),
      m_attenuation(attenuation), m_innerConeAngle(glm::cos(innerConeAngle)),
      m_outerConeAngle(glm::cos(outerConeAngle)) {}

// Use accessor method
void Light::use(const Shader &shader, size_t index) const {
  // Get the shader uniform name prefix, then call use method
  std::ostringstream ss;
  ss << "lights[" << index << "].";

  use(shader, ss.str());
}

// Access to the Light's transform
Transform &Light::transform() { return m_transform; }

glm::vec3 Light::getColor() const { return m_color; }

bool Light::isShadowCaster() const { return m_isShadowCaster; }