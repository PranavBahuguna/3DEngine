#include "Sphere.h"

// Constructor
Sphere::Sphere() {
  m_mesh = Resources::GetMesh("Sphere");
  m_shader = Resources::GetShader("Triangle");
  m_texture = Resources::GetTexture("earth.jpg");
}

// Makes the sphere spin on the current axes
ERROR Sphere::update() {
  m_angle += 0.1f;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;

  return ERROR_OK;
}
