#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Tetrahedron : public Model {
public:
  Tetrahedron();

  void update(ERROR &errCode, GLfloat deltaTime) override;
};