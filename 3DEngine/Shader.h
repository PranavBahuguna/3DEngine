#pragma once
#include "Error.h"

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  Shader(const std::string &name);
  ~Shader();

  void use() const;
  GLuint getParamId(const std::string &param, ERROR &errCode) const;

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCode);
  void compile(ERROR &errCode);
  void bindParameters();
  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr GLsizei MAX_PARAM_LENGTH = 256;

  std::unordered_map<std::string, GLuint> m_paramMap;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
