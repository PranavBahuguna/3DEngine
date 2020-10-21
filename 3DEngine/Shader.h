#pragma once

#include "Error.h"
#include "ResourceManager.h"

#include <memory>
#include <unordered_map>
#include <variant>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;
struct Preprocessor;

using ShaderSptr = std::shared_ptr<Shader>;
using UniformMap = std::unordered_map<std::string, GLuint>;
using PreprocessorData = std::variant<std::string, bool, int, float>;
using PreprocessorMap = std::unordered_map<GLuint, std::unordered_map<std::string, std::string>>;
using PreprocessorList = std::vector<Preprocessor>;

struct Preprocessor {
  Preprocessor(GLuint _type, const std::string &_name, PreprocessorData rawData) {
    type = _type;
    name = _name;

    // Try retrieving data from each preprocessor type
    if (const auto strPtr(std::get_if<std::string>(&rawData)); strPtr != nullptr)
      data = *strPtr;
    else if (const auto boolPtr(std::get_if<bool>(&rawData)); boolPtr != nullptr)
      data = std::to_string(*boolPtr);
    else if (const auto intPtr(std::get_if<int>(&rawData)); intPtr != nullptr)
      data = std::to_string(*intPtr);
    else if (const auto floatPtr(std::get_if<float>(&rawData)); floatPtr != nullptr)
      data = std::to_string(*floatPtr);
  }

  GLuint type;
  std::string name;
  std::string data;
};

class Shader : public Resource {
public:
  Shader(const std::string &name, const PreprocessorList &preprocessors = {},
         bool compileNow = true);
  Shader(const std::string &name, bool compileNow);
  ~Shader();

  void compile();

  void use() const;
  GLint getUniformId(const std::string &param) const;
  bool isCompiled() const { return m_isCompiled; }

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

private:
  void load(ERROR &errCode, const std::string &filename, GLenum type, GLuint &shaderId);
  void linkPrograms(ERROR &errCode);
  void bindUniforms();
  void preprocess(ERROR &errCode, std::string &shaderSource, GLenum type) const;

  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr int MAX_PARAM_LENGTH = 256;

  UniformMap m_uniformMap;
  PreprocessorMap m_preprocessorMap;

  bool m_isCompiled;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
};
