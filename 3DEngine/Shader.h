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

  void use() const;
  void setMVP(const glm::mat4 &mvp) const;

  GLuint getColorId() const { return m_colorId; }
  GLuint getAmbientIntensityId() const { return m_ambientIntensityId; }

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCOde);
  void compile(ERROR &errCode);
  void bindParameter(GLuint &id, const std::string &param, ERROR &errCode);
  char* getShaderErrorLog(GLuint id) const;
  char* getProgramErrorLog(GLuint id) const;

  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
  
  GLuint m_mvpId;
  GLuint m_colorId;
  GLuint m_ambientIntensityId;
};
