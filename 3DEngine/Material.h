#pragma once

#include "Shader.h"

#include <GL/glew.h>

class Material {
public:
  Material();
  Material(GLfloat sIntensity, GLfloat shine);

  void use(const Shader &shader) const;

private:
  GLfloat m_specularIntensity;
  GLfloat m_shine;
};