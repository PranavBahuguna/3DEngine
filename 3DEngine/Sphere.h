#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Sphere : public Model {
public:
  Sphere();

  void update(ERROR &errCode, GLfloat deltaTime) override;
};