#pragma once

#include "Error.h"
#include "Material.h"
#include "Resource.h"

class Mesh : public Resource {
public:
  Mesh(const std::string &name, const std::vector<GLfloat> &vertices,
       const std::vector<GLfloat> &normals, const std::vector<GLfloat> &texCoords,
       const std::vector<GLuint> &indices);
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
