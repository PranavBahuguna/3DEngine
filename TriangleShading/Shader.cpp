#include "Shader.h"

#include <fstream>
#include <vector>

// Constructor
Shader::Shader(const std::string &name) : m_progId(0), m_vertId(0), m_fragId(0) {
  // Get file paths
  const std::string vertPath = "Shaders/" + name + ".vert";
  const std::string fragPath = "Shaders/" + name + ".frag";

  // Load shaders from disk and compiles them
  LoadShader(vertPath, GL_VERTEX_SHADER, m_vertId);
  LoadShader(fragPath, GL_FRAGMENT_SHADER, m_fragId);
  CompileShaders();
}

// Destructor
Shader::~Shader() {
  glDetachShader(m_progId, m_vertId);
  glDetachShader(m_progId, m_fragId);
  glDeleteProgram(m_progId);
  glDeleteShader(m_vertId);
  glDeleteShader(m_fragId);
}

// Use this shader
void Shader::Use() const { glUseProgram(m_progId); }

// Get the id of a uniform variable
GLint Shader::GetUL(const GLchar *name) const { return glGetUniformLocation(m_progId, name); }

// Loads shader from file
void Shader::LoadShader(const std::string &filename, GLenum type, GLuint &shaderId) {
  // Open the file
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
      // Print errors and delete the shader
      printf("Error, unable to compile shader %d!\nSource:%s\n\n", id, cSource);
      PrintShaderErrorLog(id);
      glDeleteShader(id);
    }
  } else {
    printf("Error, unable to open file! %s\n", filename.c_str());
  }
}

// Compiles all loaded shaders into the program
void Shader::CompileShaders() {
  // Create program
  GLuint progId = glCreateProgram();

  if (!progId) {
    printf("Error, unable to create shader program!\n");
    return;
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
      printf("Error validating program:\n");
      PrintProgramErrorLog(progId);
    }
  } else {
    printf("Error linking program:\n");
    PrintProgramErrorLog(progId);
  }
}

// Prints the error log of a shader
void Shader::PrintShaderErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetShaderInfoLog(id, errorLength, &errorLength, &errorLog[0]);

  printf("%s\n", &errorLog[0]);
}

// Prints the error log of the program
void Shader::PrintProgramErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetProgramInfoLog(id, errorLength, &errorLength, &errorLog[0]);

  printf("%s\n", &errorLog[0]);
}