#pragma once

#include "Shader.h"

#include <GL/glew.h>
#include <assimp/material.h>

class Material {
public:
  Material(const aiMaterial &mat);

  void use(const Shader &shader) const;

private:
  aiColor3D m_ambient;
  aiColor3D m_diffuse;
  aiColor3D m_specular;
  float m_shininess;
};