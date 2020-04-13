#include "Shader.h"

#include <fstream>
#include <vector>

// Constructor
Shader::Shader(const std::string &name) : m_progId(0), m_vertId(0), m_fragId(0) {
  // Get file paths
  const std::string vertPath = "Shaders/" + name + ".vert";
  const std::string fragPath = "Shaders/" + name + ".frag";

  // Load shaders from disk and compiles them
  ERROR errCode = load(vertPath, GL_VERTEX_SHADER, m_vertId);
  if (errCode == ERROR_OK)
    load(fragPath, GL_FRAGMENT_SHADER, m_fragId);
  
  if (errCode == ERROR_OK)
    compile();

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
ERROR Shader::load(const std::string &filename, GLenum type, GLuint &shaderId) {
  // Open the file
  ERROR errCode = ERROR_OK;
  std::ifstream file(filename.c_str());

  if (file) {
    // Get shader source code from file
    std::string source;
    source.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *cSource = source.c_str();

    // Create and compile shader
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar **)&cSource, 0);
    glCompileShader(id);

    // Perform error-checking on shaders
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled) {
      shaderId = id;
    } else {
      errCode = ERROR_SHADER_COMPILE_FAILED;
      printErrorMsg(errCode, getShaderErrorLog(id));
      glDeleteShader(id);
    }
  } else {
    errCode = ERROR_FILE_OPEN_FAILED;
    printErrorMsg(errCode, filename.c_str());
  }

  return errCode;
}

// Compiles all loaded shaders into the program
ERROR Shader::compile() {
  // Create program
  ERROR errCode = ERROR_OK;
  m_progId = glCreateProgram();

  if (!m_progId) {
    errCode = ERROR_SHADER_COMPILE_FAILED;
    printErrorMsg(errCode);
    return errCode;
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

  return errCode;
}

// Prints the error log of a shader
std::string Shader::getShaderErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetShaderInfoLog(id, errorLength, &errorLength, errorLog.data());

  return errorLog.data();
}

// Prints the error log of the program
std::string Shader::getProgramErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetProgramInfoLog(id, errorLength, &errorLength, errorLog.data());
  
  return errorLog.data();
}