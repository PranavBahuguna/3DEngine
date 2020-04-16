#include "Mesh.h"

// Constructor
Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices)
    : m_VAO(0), m_VBO(0), m_IBO(0), m_indexCount(static_cast<GLsizei>(indices.size())) {

  // Bind vertex array
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind index data buffer
  glGenBuffers(1, &m_IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(indices), indices.data(),
               GL_STATIC_DRAW);

  // Bind vertex data buffer
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  // Define how to access vertex array data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * m_vertexStride, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * m_vertexStride,
                        (void *)(sizeof(vertices[0]) * 3));
  glEnableVertexAttribArray(1);

  // Unbind vertex array, index buffer and data buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Destructor
Mesh::~Mesh() {
  // Delete all buffers and vertex arrays
  if (m_IBO != 0)
    glDeleteBuffers(1, &m_IBO);
  if (m_VBO != 0)
    glDeleteBuffers(1, &m_VBO);
  if (m_VAO != 0)
    glDeleteVertexArrays(1, &m_VAO);
}

// Draws the mesh onto the screen
void Mesh::draw() const {
  // Bind vertex array and index buffer
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

  // Render object elements
  glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

  // Unbind vertex array and index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
