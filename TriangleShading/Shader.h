#pragma once
#include <GL/glew.h>

#include <string>

class Shader {
public:
  Shader(const std::string &name);
  ~Shader();

  void Use() const;
  GLint GetUL(const GLchar *name) const;

private:
  void LoadShader(const std::string &filename, GLenum type, GLuint &shaderId);
  void CompileShaders();
  void PrintShaderErrorLog(GLuint id) const;
  void PrintProgramErrorLog(GLuint id) const;

  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
