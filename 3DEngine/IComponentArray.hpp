#pragma once

#include "EntityManager.hpp"

class IComponentArray {
public:
  virtual ~IComponentArray() = default;
  virtual void entityDestroyed(Entity entity, ERROR &errCode) = 0;
};