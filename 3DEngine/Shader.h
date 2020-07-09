#pragma once

#include "Error.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

typedef std::unordered_map<std::string, GLuint> UniformMap;
typedef std::unordered_map<GLenum, std::unordered_map<std::string, std::string>> PreprocessorMap;

class Shader {
public:
  Shader(const std::string &name);
  ~Shader();

  void compile(ERROR &errCode, bool useShader = true);

  void use() const;
  GLint getUniformId(const std::string &param, ERROR &errCode) const;

  // Uniform setter utility functions
  void setBool(const std::string &name, bool value, ERROR &errCode) const;
  void setInt(const std::string &name, int value, ERROR &errCode) const;
  void setUint(const std::string &name, unsigned int value, ERROR &errCode) const;
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

  // Preprocessor setter utility functions
  void setPreprocessor(GLenum type, const std::string &name, const std::string &value);
  void setPreprocessor(GLenum type, const std::string &name, bool value);
  void setPreprocessor(GLenum type, const std::string &name, int value);
  void setPreprocessor(GLenum type, const std::string &name, float value);

  bool isCompiled() { return m_isCompiled; }

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCode);
  void linkPrograms(ERROR &errCode);
  void bindUniforms(ERROR &errCode);

  void preprocess(std::string &shaderSource, GLenum type, ERROR &errCode) const;

  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr GLsizei MAX_PARAM_LENGTH = 256;

  std::string m_name;
  UniformMap m_uniformMap;
  PreprocessorMap m_preprocessorMap;

  bool m_isCompiled;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
