#pragma once

#include <vector>

#include <GL/glew.h>

class Mesh {
public:
  Mesh(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices);
  ~Mesh();

  void Render() const;

private:
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_IBO;
  GLsizei m_indexCount;
};
