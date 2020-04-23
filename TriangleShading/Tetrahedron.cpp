#include "Tetrahedron.h"

// Constructor
Tetrahedron::Tetrahedron() {

  m_mesh = new Mesh("Tetrahedron");
  m_shader = Resources::GetShader("Triangle");
  m_texture = Resources::GetTexture("brick.png");
}

Tetrahedron::~Tetrahedron() {}

// Makes the tetrahedron spin according to the given axes
ERROR Tetrahedron::update() {
  m_angle += 0.1f;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;

  return ERROR_OK;
}
