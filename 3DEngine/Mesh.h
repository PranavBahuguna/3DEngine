#pragma once

#include "Error.h"
#include "Material.h"

#include <vector>
#include <memory>

#include <GL/glew.h>

class Mesh {
public:
  Mesh(const std::vector<float> &vertices, const std::vector<float> &normals,
       const std::vector<float> &texCoords, const std::vector<unsigned int> &indices);
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
