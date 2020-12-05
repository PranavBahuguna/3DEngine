#pragma once

#include "Error.h"
#include "GameObject.h"
#include "Shader.h"

#include <cstddef>
#include <memory>

class GameObject;

class Component {
public:
  Component(std::shared_ptr<GameObject> owner = nullptr);
  Component(const Component &component);
  virtual ~Component(){};

  virtual void init(ERROR &errCode){};
  virtual void start(ERROR &errCode){};
  virtual void update(ERROR &errCode){};
  virtual void draw(ERROR &errCode, const Shader &shader){};

  virtual std::shared_ptr<GameObject> getOwner();

protected:
  std::shared_ptr<GameObject> m_owner;
};