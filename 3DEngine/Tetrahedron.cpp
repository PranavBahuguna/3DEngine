#include "Tetrahedron.h"
#include "Timer.h"

// Constructor
Tetrahedron::Tetrahedron() : Model("Tetrahedron") {}

// Makes the model spin on the current axes
void Tetrahedron::update(ERROR &errCode) {
  m_angle += 45.0f * Timer::GetDeltaTime();
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
