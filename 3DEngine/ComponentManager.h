#pragma once

#include "ComponentArray.h"
#include "EntityManager.h"

#include <memory>
#include <unordered_map>

class ComponentManager {
public:
  ComponentManager();

  template <typename T> void registerComponent(ERROR &errCode);
  template <typename T> ComponentType getComponentType(ERROR &errCode);
  template <typename T> void addComponent(Entity entity, T &component, ERROR &errCode);
  template <typename T> void removeComponent(Entity entity, ERROR &errCode);
  template <typename T> T &getComponent(Entity entity, ERROR &errCode);
  void entityDestroyed(Entity entity);

private:
  template <typename T> std::shared_ptr<ComponentArray<T>> getComponentArray(ERROR &errCode);

  std::unordered_map<const char *, ComponentType> m_componentTypes;
  std::unordered_map<const char *, std::shared_ptr<IComponentArray>> m_componentArrays;
  ComponentType m_nextComponentType;
};