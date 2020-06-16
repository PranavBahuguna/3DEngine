#include "Mesh.h"
#include "Resources.h"

// Constructor
Mesh::Mesh(const std::vector<float> &vertices, const std::vector<float> &texCoords,
           const std::vector<float> &normals, const std::vector<unsigned int> &indices) {

  m_nIndices = (GLsizei)indices.size();

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind vertex, texture coords and normals data
  glGenBuffers(NUM_VBO, m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  // Bind index data buffer
  glGenBuffers(1, &m_IBO);
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);
  }
}

// Destructor
Mesh::~Mesh() {
  glDeleteBuffers(NUM_VBO, m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

// Draws the mesh onto the screen
void Mesh::draw() const {
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, 0);
}
