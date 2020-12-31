#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

class ECSEngine {
public:
  ECSEngine();

  // Entity methods
  Entity createEntity(ERROR &errCode);
  void destroyEntity(Entity entity, ERROR &errCode);

  // Component methods
  template <typename T> void registerComponent(ERROR &errCode);
  template <typename T> void addComponent(Entity entity, T &component, ERROR &errCode);
  template <typename T> void removeComponent(Entity entity, ERROR &errCode);
  template <typename T> T &getComponent(Entity entity, ERROR &errCode);
  template <typename T> ComponentType getComponentType(ERROR &errCode);

  // System methods
  template <typename T> std::shared_ptr<T> registerSystem(ERROR &errCode);
  template <typename T> void setSystemSignature(Signature signature, ERROR &errCode);

private:
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<ComponentManager> m_componentManager;
  std::unique_ptr<SystemManager> m_systemManager;
};