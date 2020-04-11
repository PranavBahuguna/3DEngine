#pragma once
#include "Error.h"

#include <string>

#include <GL/glew.h>

class Shader {
public:
  Shader(const std::string &name);
  ~Shader();

  void use() const { glUseProgram(m_progId); }
  GLint getUL(const GLchar *name) const { return glGetUniformLocation(m_progId, name); }

private:
  ERROR load(const std::string &filename, GLenum type, GLuint &shaderId);
  ERROR compile();
  void printShaderErrorLog(GLuint id, ERROR errCode) const;
  void printProgramErrorLog(GLuint id, ERROR errCode) const;

  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
