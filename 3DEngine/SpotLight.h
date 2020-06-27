#pragma once

#include "PointLight.h"

class SpotLight : public PointLight {
public:
  SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
            const glm::vec3 &position, const glm::vec3 &coneDir, GLfloat innerConeAngle,
            GLfloat outerConeAngle, GLfloat constant, GLfloat linear, GLfloat quadratic);

  void use(const Shader &shader, ERROR &errCode) const override;

private:
  glm::vec3 m_coneDir;
  GLfloat m_innerConeAngle;
  GLfloat m_outerConeAngle;
};