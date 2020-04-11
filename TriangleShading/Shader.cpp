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
      printShaderErrorLog(id, errCode);
      glDeleteShader(id);
    }
  } else {
    errCode = ERROR_FILE_OPEN_FAILED;
    printErrorMsg(errCode, 1, filename.c_str());
  }

  return errCode;
}

// Compiles all loaded shaders into the program
ERROR Shader::compile() {
  // Create program
  ERROR errCode = ERROR_OK;
  GLuint progId = glCreateProgram();

  if (!progId) {
    errCode = ERROR_SHADER_COMPILE_FAILED;
    printErrorMsg(errCode);
    return errCode;
  }

  // Attach all shaders to the program
  glAttachShader(progId, m_fragId);
  glAttachShader(progId, m_vertId);

  // Link the program
  glLinkProgram(progId);
  GLint isLinked, isValid;
  glGetProgramiv(progId, GL_LINK_STATUS, &isLinked);

  if (isLinked) {
    // Validate the program
    glValidateProgram(progId);
    glGetProgramiv(progId, GL_VALIDATE_STATUS, &isValid);

    if (isValid) {
      m_progId = progId;
    } else {
      errCode = ERROR_SHADER_PROGRAM_INVALID;
      printProgramErrorLog(progId, errCode);
    }
  } else {
    errCode = ERROR_SHADER_PROGRAM_LINKING_FAILED;
    printProgramErrorLog(progId, errCode);
  }

  return errCode;
}

// Prints the error log of a shader
void Shader::printShaderErrorLog(GLuint id, ERROR errCode) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetShaderInfoLog(id, errorLength, &errorLength, &errorLog[0]);

  printErrorMsg(errCode, 2, id, errorLog.data());
}

// Prints the error log of the program
void Shader::printProgramErrorLog(GLuint id, ERROR errCode) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetProgramInfoLog(id, errorLength, &errorLength, &errorLog[0]);

  printErrorMsg(errCode, 2, id, errorLog.data());
}