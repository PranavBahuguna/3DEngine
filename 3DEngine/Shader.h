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

  // Parameter setter utility functions
  void setBool(const std::string &name, bool value, ERROR &errCode) const;
  void setInt(const std::string &name, int value, ERROR &errCode) const;
  void setFloat(const std::string &name, float value, ERROR &errCode) const;
  void setVec2(const std::string &name, const glm::vec2 &value, ERROR &errCode) const;
  void setVec2(const std::string &name, GLfloat x, GLfloat y, ERROR &errCode) const;
  void setVec3(const std::string &name, const glm::vec3 &value, ERROR &errCode) const;
  void setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z, ERROR &errCode) const;
  void setVec4(const std::string &name, const glm::vec4 &value, ERROR &errCode) const;
  void setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w,
               ERROR &errCode) const;
  void setMat2(const std::string &name, const glm::mat2 &value, ERROR &errCode) const;
  void setMat3(const std::string &name, const glm::mat3 &value, ERROR &errCode) const;
  void setMat4(const std::string &name, const glm::mat4 &value, ERROR &errCode) const;

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCode);
  void compile(ERROR &errCode);
  void bindParameters();

  GLint getParamId(const std::string &param, ERROR &errCode) const;
  
  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr GLsizei MAX_PARAM_LENGTH = 256;

  std::unordered_map<std::string, GLuint> m_paramMap;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
