#include "Cube.h"

// Constructor
Cube::Cube() : Model("Cube") {}

// Makes the model spin on the current axes
void Cube::update(ERROR &errCode, GLfloat deltaTime) {
  m_angle += 45.0f * deltaTime;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
