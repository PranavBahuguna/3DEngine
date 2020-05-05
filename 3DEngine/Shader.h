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
  void setModel(const glm::mat4 &model) const;
  void setView(const glm::mat4 &view) const;
  void setProjection(const glm::mat4 &projection) const;

  GLuint getColorId() const { return m_colorId; }
  GLuint getLightDirectionId() const { return m_lightDirectionId; }
  GLuint getAmbientIntensityId() const { return m_ambientIntensityId; }
  GLuint getDiffuseIntensityId() const { return m_diffuseIntensityId; }

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCOde);
  void compile(ERROR &errCode);
  void bindParameter(GLuint &id, const std::string &param, ERROR &errCode);
  char* getShaderErrorLog(GLuint id) const;
  char* getProgramErrorLog(GLuint id) const;

  // Shader program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;
  
  // Shader parameter ids
  GLuint m_modelId;
  GLuint m_viewId;
  GLuint m_projectionId;
  GLuint m_colorId;
  GLuint m_lightDirectionId;
  GLuint m_ambientIntensityId;
  GLuint m_diffuseIntensityId;
};
