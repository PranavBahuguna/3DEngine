#pragma once

#include "Light.h"

class PointLight : public Light {
public:
  PointLight(const Transform &transform, const glm::vec3 &color, const Phong &phong,
             const Attenuation &attenuation, bool isShadowCaster = false);

  void setLightSpaceMatrix(const Shader &shader) const override;
  void update(ERROR &errCode) override;

private:
  void use(const Shader &shader, const std::string &prefix) const override;
};