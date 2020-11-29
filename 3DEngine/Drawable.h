#pragma once

#include "Shader.h"
#include "Transform.h"
#include "Component.h"

static GLuint id = 0;

class Drawable : public Component {
public:
  Drawable();
  Drawable(std::shared_ptr<GameObject> owner);
  virtual ~Drawable() = default;

  GLuint _id;

  void setActive(bool value);
  bool isActive() const;
  Transform &transform();

  virtual void draw(ERROR &errCode, const Shader &shader) const = 0;

protected:
  bool m_isActive;
};