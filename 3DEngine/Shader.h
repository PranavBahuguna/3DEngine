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
  void setModel(const glm::mat4 &model) const;
  void setViewPos(const glm::vec3 &viewPos) const;

  GLuint getColorId() const { return m_colorId; }
  GLuint getAmbientIntensityId() const { return m_ambientIntensityId; }
  GLuint getLightPosId() const { return m_lightPosId; }
  GLuint getDiffuseIntensityId() const { return m_diffuseIntensityId; }
  GLuint getSpecularIntensityId() const { return m_specularIntensityId; }
  GLuint getShineId() const { return m_shineId; }

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCOde);
  void compile(ERROR &errCode);
  void bindParameter(GLuint &id, const std::string &param, ERROR &errCode);
  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;

  // Parameter ids
  GLuint m_mvpId;
  GLuint m_modelId;
  GLuint m_viewPosId;
  GLuint m_colorId;
  GLuint m_ambientIntensityId;
  GLuint m_lightPosId;
  GLuint m_diffuseIntensityId;
  GLuint m_specularIntensityId;
  GLuint m_shineId;
};
