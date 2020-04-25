#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Tetrahedron : public Model {
public:
  Tetrahedron();
  ~Tetrahedron(){};

  ERROR update() override;
};