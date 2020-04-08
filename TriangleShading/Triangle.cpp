#include "Triangle.h"

// Constructor
Triangle::Triangle() : m_VAO(0), m_VBO(0) {
  GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  // Bind vertex array
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind vertex data buffer
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Define how to access vertex array data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // Unbind vertex aarray and data buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Binds triangle's vertices to the vertex array object
void Triangle::BindVertices() const { glBindVertexArray(m_VAO); }
