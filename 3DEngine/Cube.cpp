#include "Cube.h"
#include "Timer.h"

// Constructor
Cube::Cube() : Model("Cube") {}

// Makes the model spin on the current axes
void Cube::update(ERROR &errCode) {
  m_angle += 45.0f * Timer::GetDeltaTime();
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
