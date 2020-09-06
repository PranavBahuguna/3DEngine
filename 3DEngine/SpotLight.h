#pragma once

#include "Light.h"

class SpotLight : public Light {
public:
  SpotLight(const glm::vec3 &position, const glm::vec3 &color, float ambient, float diffuse,
            float specular, float constant, float linear, float quadratic, const glm::vec3 &coneDir,
            float innerConeAngle, float outerConeAngle, bool isShadowCaster = false);

  void setLightSpaceMatrix(const Shader &shader) const override;
  void update(ERROR &errCode) override;

private:
  void use(const Shader &shader, const std::string &prefix) const override;

  glm::mat4 m_lightSpaceMatrix;
};