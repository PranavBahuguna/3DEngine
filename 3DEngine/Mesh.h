#pragma once

#include "Error.h"

#include <vector>

#include <GL/glew.h>

class Mesh {
public:
  Mesh(const std::string &name);
  ~Mesh();

  void draw() const;

private:
  static const int NUM_VBO = 2;
  static const GLuint VERTEX_STRIDE = 5;

  ERROR load(const std::string &filepath, std::vector<GLfloat> &vertices,
             std::vector<GLfloat> &uvs) const;

  GLuint m_VAO;
  GLuint m_VBO[NUM_VBO];

  GLsizei m_numVerts;
};
