#pragma once

#include "Object.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Tetrahedron : public Object {
public:
  Tetrahedron();
  ~Tetrahedron();

  ERROR update() override;
};