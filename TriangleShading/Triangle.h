#pragma once
#include <GL/glew.h>

#include <string>

class Triangle {
public:
  Triangle();

  void BindVertices() const;

private:
  GLuint m_VAO;
  GLuint m_VBO;
};
