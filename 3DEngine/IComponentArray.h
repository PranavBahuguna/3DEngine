#pragma once

#include "EntityManager.h"

class IComponentArray {
public:
  virtual ~IComponentArray() = default;
  virtual void entityDestroyed(Entity entity) = 0;
};