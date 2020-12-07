#pragma once

#include "Error.h"
#include "GameObject.h"
#include "Shader.h"

#include <memory>

class GameObject;

class Component {
public:
  Component(const std::shared_ptr<GameObject> &owner = nullptr);
  Component(const Component &component);
  virtual ~Component(){};

  virtual void init(ERROR &errCode){};
  virtual void start(ERROR &errCode){};
  virtual void update(ERROR &errCode){};
  virtual void draw(ERROR &errCode, const Shader &shader){};

protected:
  std::shared_ptr<GameObject> m_owner;
};