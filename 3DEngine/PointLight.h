#pragma once

#include "Light.h"

class PointLight : public Light {
public:
  PointLight(const glm::vec3 &position, const glm::vec3 &color, float ambient, float diffuse,
             float specular, float constant, float linear, float quadratic,
             bool isShadowCaster = false);

  void setLightSpaceMatrix(const Shader &shader) const override;
  void update(ERROR &errCode) override;

private:
  void use(const Shader &shader, const std::string &prefix) const override;
};