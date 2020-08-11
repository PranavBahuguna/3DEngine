#pragma once

#include "Error.h"
#include "ResourceManager.h"

#include <memory>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;
using UniformMap = std::unordered_map<std::string, GLuint>;
using PreprocessorMap = std::unordered_map<GLenum, std::unordered_map<std::string, std::string>>;
using ShaderSptr = std::shared_ptr<Shader>;

class Shader : public Resource {
public:
  Shader(const std::string &name);
  ~Shader();

  void compile(bool useShader = true);

  void use() const;
  GLint getUniformId(const std::string &param) const;

  // Uniform setter utility functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setUint(const std::string &name, unsigned int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, GLfloat x, GLfloat y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
  void setMat2(const std::string &name, const glm::mat2 &value) const;
  void setMat3(const std::string &name, const glm::mat3 &value) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;

  // Preprocessor setter utility functions
  void setPreprocessor(GLenum type, const std::string &name, const std::string &value);
  void setPreprocessor(GLenum type, const std::string &name, bool value);
  void setPreprocessor(GLenum type, const std::string &name, int value);
  void setPreprocessor(GLenum type, const std::string &name, float value);

  bool isCompiled() { return m_isCompiled; }

private:
  void load(ERROR &errCode, const std::string &filename, GLenum type, GLuint &shaderId);
  void linkPrograms(ERROR &errCode);
  void bindUniforms();
  void preprocess(ERROR &errCode, std::string &shaderSource, GLenum type) const;

  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr GLsizei MAX_PARAM_LENGTH = 256;

  UniformMap m_uniformMap;
  PreprocessorMap m_preprocessorMap;

  bool m_isCompiled;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
