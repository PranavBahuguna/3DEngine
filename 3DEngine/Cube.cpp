#include "Cube.h"

// Constructor
Cube::Cube() {
  m_mesh = Resources::GetMesh("Cube");
  m_shader = Resources::GetShader("Triangle");
  m_texture = Resources::GetTexture("brick.png");
}

// Makes the model spin on the current axes
void Cube::update(ERROR &errCode) {
  m_angle += 0.1f;
  if (m_angle >= 360.0f)
    m_angle -= 360.0f;
}
