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
  void setViewPos(const glm::vec3 &viewPos) const;

  const GLuint* getLightIds() const { return m_lightId; }
  const GLuint *getMaterialIds() const { return m_materialId; }

private:
  void load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCOde);
  void compile(ERROR &errCode);
  void bindParameter(GLuint &id, const std::string &param, ERROR &errCode);
  char *getShaderErrorLog(GLuint id) const;
  char *getProgramErrorLog(GLuint id) const;

  static constexpr int NUM_LIGHT_IDS = 4;
  static constexpr int NUM_MATERIAL_IDS = 4;

  // Program ids
  GLuint m_progId;
  GLuint m_vertId;
  GLuint m_fragId;

  // Parameter ids
  GLuint m_modelId;
  GLuint m_viewId;
  GLuint m_projectionId;
  GLuint m_viewPosId;
  GLuint m_lightId[NUM_LIGHT_IDS];
  GLuint m_materialId[NUM_MATERIAL_IDS];
};
