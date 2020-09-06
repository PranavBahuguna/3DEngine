#pragma once

#include "Shader.h"

class Light;
using LightSptr = std::shared_ptr<Light>;

class Light {
public:
  Light(const glm::vec3 &position, bool isDir, const glm::vec3 &color, float ambient,
        float diffuse, float specular, bool isShadowCaster = false, float constant = 0.0f,
        float linear = 0.0f, float quadratic = 0.0f, const glm::vec3 &coneDir = glm::vec3(0.0f),
        float innerConeAngle = 0.0f, float outerConeAngle = 0.0f);

  virtual void use(const Shader &shader, size_t index) const;
  virtual void setLightSpaceMatrix(const Shader &shader) const = 0;
  virtual void update(ERROR &errCode) = 0;

  glm::vec3 getPosition() const { return m_position; };
  glm::vec3 getColor() const { return m_color; }
  bool isShadowCaster() const { return m_isShadowCaster; }

protected:
  virtual void use(const Shader &shader, const std::string &prefix) const = 0;

  glm::vec3 m_position;
  float m_positionW;
  glm::vec3 m_color;
  float m_ambient;
  float m_diffuse;
  float m_specular;
  bool m_isShadowCaster;
  float m_constant;
  float m_linear;
  float m_quadratic;
  glm::vec3 m_coneDir;
  float m_innerConeAngle;
  float m_outerConeAngle;
};