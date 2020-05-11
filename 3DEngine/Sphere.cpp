#include "Sphere.h"

// Constructor
Sphere::Sphere() : Model("Sphere") {}

// Makes the model spin on the current axes
void Sphere::update(ERROR &errCode) {
  m_angle += 0.1f;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
