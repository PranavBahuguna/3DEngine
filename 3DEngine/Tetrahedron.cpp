#include "Tetrahedron.h"

// Constructor
Tetrahedron::Tetrahedron() : Model("Tetrahedron") {}

// Makes the model spin on the current axes
void Tetrahedron::update(ERROR &errCode, GLfloat deltaTime) {
  m_angle += 45.0f * deltaTime;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
