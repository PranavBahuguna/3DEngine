#pragma once

#include "Error.h"
#include "Material.h"

#include <memory>
#include <vector>

#include <GL/glew.h>

class Mesh {
public:
  Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &normals,
       const std::vector<GLfloat> &texCoords, const std::vector<GLuint> &indices);
  ~Mesh();

  void draw() const;

private:
  static const int NUM_VBO = 3;
  static const GLuint VERTEX_STRIDE = 5;

  GLuint m_IBO;
  GLuint m_VAO;
  GLuint m_VBO[NUM_VBO];
  GLsizei m_nIndices;
};
