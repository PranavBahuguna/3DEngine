#pragma once

#include "Projection.h"

class View : public Projection {
public:
  View();
  View(float left, float right, float bottom, float top, float zNear, float zFar);
  View(float fovy, float aspectRatio, float zNear, float zFar);

private:
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 up;
};