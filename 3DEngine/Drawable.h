#pragma once

#include "Shader.h"
#include "Transform.h"

static GLuint id = 0;

class Drawable {
public:
  Drawable(const Transform &transform);
  virtual ~Drawable() = default;

  GLuint _id;

  void setActive(bool value);
  bool isActive() const;
  Transform &transform();

  virtual void draw(ERROR &errCode, const Shader &shader) const = 0;

protected:
  Transform m_transform;
  bool m_isActive;
};