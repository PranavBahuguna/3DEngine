#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object {
public:
  Object();
  virtual ~Object();

  unsigned int _id; 

  virtual ERROR update() = 0;

  void setPosition(const glm::vec3 &pos) { m_pos = pos; }
  void setRotation(float angle, const glm::vec3 &euler) {
    m_angle = angle;
    m_euler = euler;
  }
  void setScale(const glm::vec3 &scale) { m_scale = scale; }

  ERROR draw(const Camera &camera);

  glm::mat4 getModel() const;

protected:
  glm::vec3 m_pos;
  float m_angle;
  glm::vec3 m_euler;
  glm::vec3 m_scale;

  Mesh *m_mesh;
  Texture *m_texture;
  Shader *m_shader;
};