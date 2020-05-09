#include "Shader.h"

#include <GL\GL.h>

#include <fstream>
#include <vector>

// Constructor
Shader::Shader(const std::string &name) : m_progId(0), m_vertId(0), m_fragId(0) {
  // Get file paths
  const std::string vertPath = "Shaders/" + name + ".vert";
  const std::string fragPath = "Shaders/" + name + ".frag";

  // Load shaders from disk and compiles them
  ERROR errCode = ERROR_OK;
  load(vertPath, GL_VERTEX_SHADER, m_vertId, errCode);
  load(fragPath, GL_FRAGMENT_SHADER, m_fragId, errCode);
  compile(errCode);

  // Bind parameters from shader files
  bindParameter(m_mvpId, "mvp", errCode);
  bindParameter(m_modelId, "model", errCode);
  bindParameter(m_viewPosId, "viewPos", errCode);

  bindParameter(m_lightId[0], "light.ambient", errCode);
  bindParameter(m_lightId[1], "light.diffuse", errCode);
  bindParameter(m_lightId[2], "light.specular", errCode);
  bindParameter(m_lightId[3], "light.position", errCode);

  bindParameter(m_materialId[0], "material.ambient", errCode);
  bindParameter(m_materialId[1], "material.diffuse", errCode);
  bindParameter(m_materialId[2], "material.specular", errCode);
  bindParameter(m_materialId[3], "material.shininess", errCode);

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while loading shader.");
}

// Destructor
Shader::~Shader() {
  glDetachShader(m_progId, m_vertId);
  glDetachShader(m_progId, m_fragId);
  glDeleteProgram(m_progId);
  glDeleteShader(m_vertId);
  glDeleteShader(m_fragId);
}

// Loads shader from file
void Shader::load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  // Open the file
  std::ifstream file(filename.c_str());

  if (file) {
    // Get shader source code from file
    std::string source;
    source.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *cSource = source.c_str();

    // Create and compile shader
    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, (const GLchar **)&cSource, 0);
    glCompileShader(shaderId);

    // Perform error-checking on shaders
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
      errCode = ERROR_SHADER_COMPILE_FAILED;
      printErrorMsg(errCode, shaderId, getShaderErrorLog(shaderId));
    }
  } else {
    errCode = ERROR_FILE_OPEN_FAILED;
    printErrorMsg(errCode, filename.c_str());
  }
}

// Compiles all loaded shaders into the program
void Shader::compile(ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  m_progId = glCreateProgram();
  if (!m_progId) {
    errCode = ERROR_SHADER_PROGRAM_CREATE_FAILED;
    printErrorMsg(errCode, m_progId);
    return;
  }

  // Attach all shaders to the program
  glAttachShader(m_progId, m_fragId);
  glAttachShader(m_progId, m_vertId);

  // Link the program
  glLinkProgram(m_progId);
  GLint isLinked, isValid;
  glGetProgramiv(m_progId, GL_LINK_STATUS, &isLinked);

  if (isLinked) {
    // Validate the program
    glValidateProgram(m_progId);
    glGetProgramiv(m_progId, GL_VALIDATE_STATUS, &isValid);

    if (!isValid)
      errCode = ERROR_SHADER_PROGRAM_INVALID;
  } else {
    errCode = ERROR_SHADER_PROGRAM_LINKING_FAILED;
  }

  // Report any errors that occurred
  if (errCode != ERROR_OK)
    printErrorMsg(errCode, getProgramErrorLog(m_progId));
}

// Use this shader
void Shader::use() const { glUseProgram(m_progId); }

// Sets the shader's mvp parameter
void Shader::setMVP(const glm::mat4 &mvp) const {
  glUniformMatrix4fv(m_mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
}

// Sets the shader's model parameter
void Shader::setModel(const glm::mat4 &model) const {
  glUniformMatrix4fv(m_modelId, 1, GL_FALSE, glm::value_ptr(model));
}

// Sets the shader's viewPos parameter
void Shader::setViewPos(const glm::vec3 &viewPos) const {
  glUniform3f(m_viewPosId, viewPos.x, viewPos.y, viewPos.z);
}

// Binds a parameter to a shader variable
void Shader::bindParameter(GLuint &id, const std::string &param, ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  id = glGetUniformLocation(m_progId, param.c_str());
  if (id == GL_INVALID_VALUE) {
    errCode = ERROR_SHADER_MISSING_PARAMETER;
    printErrorMsg(errCode, param);
  }
}

// Prints the error log of a shader
char *Shader::getShaderErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetShaderInfoLog(id, errorLength, &errorLength, errorLog.data());

  // Copy errLog to output
  size_t len = errorLog.size() + 1;
  char *outStr = new char[len];
  strcpy_s(outStr, len, errorLog.data());

  return outStr;
}

// Prints the error log of the program
char *Shader::getProgramErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetProgramInfoLog(id, errorLength, &errorLength, errorLog.data());

  // Copy errLog to output
  size_t len = errorLog.size() + 1;
  char *outStr = new char[len];
  strcpy_s(outStr, len, errorLog.data());

  return outStr;
}
