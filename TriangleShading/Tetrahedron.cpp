#include "Tetrahedron.h"

// Constructor
Tetrahedron::Tetrahedron() {

  // Initialise vertices and indices
  // xyz/uv format
  GLfloat h = (sqrt(6.0f) / 3.0f) * 2.0f;
  std::vector<GLfloat> vertices = {
      -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, // front-left
      0.0f,  h,    0.0f,  0.25f, 0.5f, // top
      1.0f,  0.0f, -1.0f, 0.5f,  0.0f, // front-right
      0.0f,  0.0f, 1.0f,  0.75f, 0.5f  // back
  };

  std::vector<unsigned int> indices = {
      0, 3, 1, // back-left
      1, 3, 2, // back-right
      2, 3, 0, // bottom
      0, 1, 2  // front
  };

  m_mesh = new Mesh(vertices, indices);
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
