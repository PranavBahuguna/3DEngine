#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
  DirectionalLight(const glm::vec3 &position, const glm::vec3 &color, float ambient, float diffuse,
                   float specular, bool isShadowCaster = false);

private:
  virtual void use(const Shader &shader, const std::string &prefix) const override;
};