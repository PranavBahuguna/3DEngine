#include "DirectionalLight.h"

// Constructor
DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                                   const glm::vec3 &specular, const glm::vec3 &direction)
    : Light(ambient, diffuse, specular), m_direction(direction) {}

// Supply shader with spot light data
void DirectionalLight::use(const Shader &shader, ERROR &errCode) const {
  Light::use(shader, errCode);

  // w-value of 0 indicates to shader that this is a direction
  glUniform4f(shader.getParamId("light.position", errCode), m_direction.x, m_direction.y,
              m_direction.z, 0.0f);
}