#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
  DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                   const glm::vec3 &direction);

  void use(const Shader &shader, ERROR &errCode) const override;

private:
  glm::vec3 m_direction;
};