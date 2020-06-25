#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Cube : public Model {
public:
  Cube();

  void update(ERROR &errCode, GLfloat deltaTime) override;
};