#pragma once

#include "Shader.h"

static GLuint id = 0;

class Drawable {
public:
  Drawable(const glm::vec3 &pos = glm::vec3(0.0f));
  virtual ~Drawable() = default;

  GLuint _id;

  virtual void draw(ERROR &errCode, const Shader &shader) const = 0;

  void setActive(bool value);

  bool isActive() const;
  glm::vec3 getPos() const;

protected:
  bool m_isActive;
  glm::vec3 m_pos;
};