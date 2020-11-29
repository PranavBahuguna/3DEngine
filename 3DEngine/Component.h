#pragma once

#include "Error.h"
#include "GameObject.h"

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

protected:
  std::shared_ptr<GameObject> m_owner;
};