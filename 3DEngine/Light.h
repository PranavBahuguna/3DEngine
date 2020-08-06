#pragma once

#include "Shader.h"

enum class LightType { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

class Light {
public:
  Light(LightType type, const glm::vec3 &position, const glm::vec3 &color, float ambient,
        float diffuse, float specular, float constant = 0.0f, float linear = 0.0f,
        float quadratic = 0.0f, const glm::vec3 &coneDir = glm::vec3(0.0f),
        float innerConeAngle = 0.0f, float outerConeAngle = 0.0f);

  void use(const Shader &shader, size_t index) const;

  LightType getType() const { return m_type; };
  glm::vec3 getColor() const { return m_color; };
  glm::vec4 getPosition() const { return m_position; };

private:
  LightType m_type;

  glm::vec4 m_position;
  glm::vec3 m_color;

  float m_ambient;
  float m_diffuse;
  float m_specular;

  float m_constant;
  float m_linear;
  float m_quadratic;

  glm::vec3 m_coneDir;
  float m_innerConeAngle;
  float m_outerConeAngle;
};