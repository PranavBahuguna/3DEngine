#pragma once

#include "Shader.h"

enum class LightType { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

class Light {
public:
  Light(LightType type, const glm::vec3 &ambient, const glm::vec3 &diffuse,
        const glm::vec3 &specular, const glm::vec3 &position, GLfloat constant = 0.0f,
        GLfloat linear = 0.0f, GLfloat quadratic = 0.0f, const glm::vec3 &coneDir = glm::vec3(0.0f),
        GLfloat innerConeAngle = 0.0f, GLfloat outerConeAngle = 0.0f);

  void use(const Shader &shader, size_t index, ERROR &errCode) const;
  void update(ERROR &errCode){};

private:
  LightType m_type;

  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  glm::vec4 m_position;

  GLfloat m_constant;
  GLfloat m_linear;
  GLfloat m_quadratic;

  glm::vec3 m_coneDir;
  GLfloat m_innerConeAngle;
  GLfloat m_outerConeAngle;
};