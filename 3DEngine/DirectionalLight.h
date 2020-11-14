#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
  DirectionalLight(const Transform &transform, const glm::vec3 &color, const Phong &phong,
                   bool isShadowCaster = false);

  void setLightSpaceMatrix(const Shader &shader) const override;
  void update(ERROR &errCode) override;

private:
  virtual void use(const Shader &shader, const std::string &prefix) const override;
};