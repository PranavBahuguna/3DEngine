#include "Tetrahedron.h"

// Constructor
Tetrahedron::Tetrahedron() : Model("Tetrahedron") {}

// Makes the model spin on the current axes
void Tetrahedron::update(ERROR &errCode) {
  m_angle += 0.1f;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
