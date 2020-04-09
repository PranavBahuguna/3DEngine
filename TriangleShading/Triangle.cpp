#include "Triangle.h"

// Constructor
Triangle::Triangle() : m_VAO(0), m_VBO(0), m_IBO(0) {

  unsigned int indices[] = {
    0, 3, 1, 
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };

  GLfloat height = sqrt(4.0f / 3.0f);

  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f, // bottom-left
    0.0f, 0.0f, height, // above
    1.0f, -1.0f, 0.0f,  // bottom-right
    0.0f, 1.0f, 0.0f,   // top
  };

  // Bind vertex array
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind index data buffer
  glGenBuffers(1, &m_IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Bind vertex data buffer
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Define how to access vertex array data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // Unbind vertex array, index buffer and data buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Draws the triangle to the screen
void Triangle::Draw() const {

  // Bind vertex array and index buffer
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

  // Draw object elements
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  
  // Unbind vertex array and index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
