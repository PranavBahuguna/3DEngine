#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model {
public:
  Model();
  virtual ~Model(){};

  unsigned int _id;

  virtual ERROR update() { return ERROR_OK; };
  ERROR draw(const Camera &camera);

  void setPosition(const glm::vec3 &pos) { m_pos = pos; }
  void setRotation(const glm::vec3 &euler, float angle) {
    m_euler = euler;
    m_angle = angle;
  }
  void setScale(const glm::vec3 &scale) { m_scale = scale; }

  glm::mat4 getMatrix() const;

protected:
  glm::vec3 m_pos;
  glm::vec3 m_euler;
  float m_angle;
  glm::vec3 m_scale;

  std::shared_ptr<Mesh> m_mesh;
  std::shared_ptr<Texture> m_texture;
  std::shared_ptr<Shader> m_shader;
};