#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Cube : public Model {
public:
  Cube();
  ~Cube(){};

  ERROR update() override;
};