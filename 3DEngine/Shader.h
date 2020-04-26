#pragma once
#include "Error.h"

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  Shader(const std::string &name);
  ~Shader();

  void use() const { glUseProgram(m_progId); }
  void setMVP(const glm::mat4 &mvp) const {
    glUniformMatrix4fv(m_mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
  }

private:
  ERROR load(const std::string &filename, GLenum type, GLuint &shaderId);
  ERROR compile();
  ERROR getMVPParameter();
  std::string getShaderErrorLog(GLuint id) const;
  std::string getProgramErrorLog(GLuint id) const;

  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
  GLint m_mvpId;
};
