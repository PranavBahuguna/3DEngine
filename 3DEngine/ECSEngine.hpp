#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

class ECSEngine {
public:
  // Entity methods
  static Entity CreateEntity(ERROR &errCode) { return m_entityManager->create(errCode); }

  static void DestroyEntity(Entity entity, ERROR &errCode) {
    m_entityManager->destroy(entity, errCode);

    if (errCode == ERROR_OK) {
      m_componentManager->entityDestroyed(entity, errCode);
      m_systemManager->entityDestroyed(entity);
    }
  }

  // Component methods
  template <typename T> static void RegisterComponent(ERROR &errCode) {
    m_componentManager->registerComponent<T>(errCode);
  }

  template <typename T>
  static void AddComponent(Entity entity, const T &component, ERROR &errCode) {
    m_componentManager->addComponent<T>(entity, component, errCode);
    if (errCode != ERROR_OK)
      return;

    auto signature = m_entityManager->getSignature(entity, errCode);
    auto componentType = m_componentManager->getComponentType<T>(errCode);
    if (errCode != ERROR_OK)
      return;

    signature.set(componentType, true);
    m_entityManager->setSignature(entity, signature, errCode);
    if (errCode != ERROR_OK)
      return;

    m_systemManager->entitySignatureChanged(entity, signature);
  }

  template <typename T> static void RemoveComponent(Entity entity, ERROR &errCode) {
    m_componentManager->removeComponent<T>(entity, errCode);
    if (errCode != ERROR_OK)
      return;

    auto signature = m_entityManager->getSignature(entity, errCode);
    auto componentType = m_componentManager->getComponentType<T>(errCode);
    if (errCode != ERROR_OK)
      return;

    signature.set(componentType, false);
    m_entityManager->setSignature(entity, signature, errCode);
    if (errCode != ERROR_OK)
      return;

    m_systemManager->entitySignatureChanged(entity, signature, errCode);
  }

  template <typename T> static T &GetComponent(Entity entity, ERROR &errCode) {
    return m_componentManager->getComponent<T>(entity, errCode);
  }

  template <typename T> static ComponentType GetComponentType(ERROR &errCode) {
    return m_componentManager->getComponentType<T>(errCode);
  }

  // System methods
  template <typename T> static std::shared_ptr<T> RegisterSystem(ERROR &errCode) {
    return m_systemManager->registerSystem<T>(errCode);
  }

  template <typename T> static void SetSystemSignature(Signature signature, ERROR &errCode) {
    m_systemManager->setSignature<T>(signature, errCode);
  }

private:
  ECSEngine(){};
  ECSEngine(const ECSEngine &){};
  ECSEngine &operator=(const ECSEngine &){};

  inline static std::unique_ptr<EntityManager> m_entityManager = std::make_unique<EntityManager>();
  inline static std::unique_ptr<ComponentManager> m_componentManager =
      std::make_unique<ComponentManager>();
  inline static std::unique_ptr<SystemManager> m_systemManager = std::make_unique<SystemManager>();
};