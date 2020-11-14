#pragma once

#include "Shader.h"
#include "Transform.h"

class Light;
using LightSptr = std::shared_ptr<Light>;

struct Phong {
  float ambient;
  float diffuse;
  float specular;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};

class Light {
public:
  Light(const Transform &transform, const glm::vec3 &color, const Phong &phong,
        bool isShadowCaster = false, const Attenuation &attenuation = {0.0f, 0.0f, 0.0f},
        float innerConeAngle = 0.0f, float outerConeAngle = 0.0f);

  Transform &transform();
  glm::vec3 getColor() const;
  bool isShadowCaster() const;

  virtual void use(const Shader &shader, size_t index) const;
  virtual void setLightSpaceMatrix(const Shader &shader) const = 0;
  virtual void update(ERROR &errCode) = 0;

protected:
  virtual void use(const Shader &shader, const std::string &prefix) const = 0;

  Transform m_transform;
  glm::vec3 m_color;
  Phong m_phong;
  bool m_isShadowCaster;
  Attenuation m_attenuation;
  float m_innerConeAngle;
  float m_outerConeAngle;
};