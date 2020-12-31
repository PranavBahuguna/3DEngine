#include "ECSEngine.h"

ECSEngine::ECSEngine()
    : m_entityManager(std::make_unique<EntityManager>()),
      m_componentManager(std::make_unique<ComponentManager>()),
      m_systemManager(std::make_unique<SystemManager>()) {}

Entity ECSEngine::createEntity(ERROR &errCode) { return m_entityManager->create(errCode); }

void ECSEngine::destroyEntity(Entity entity, ERROR &errCode) {
  m_entityManager->destroy(entity, errCode);

  if (errCode == ERROR_OK) {
    m_componentManager->entityDestroyed(entity);
    m_systemManager->entityDestroyed(entity);
  }
}

template <typename T> void ECSEngine::registerComponent(ERROR &errCode) {
  m_componentManager->registerComponent<T>(errCode);
}

template <typename T> void ECSEngine::addComponent(Entity entity, T &component, ERROR &errCode) {
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

  m_systemManager->entitySignatureChanged(entity, signature, errCode);
}

template <typename T> void ECSEngine::removeComponent(Entity entity, ERROR &errCode) {
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

template <typename T> T &ECSEngine::getComponent(Entity entity, ERROR &errCode) {
  return m_componentManager->getComponent<T>(entity, errCode);
}

template <typename T> ComponentType ECSEngine::getComponentType(ERROR &errCode) {
  return m_componentManager->getComponentType<T>(errCode);
}

template <typename T> std::shared_ptr<T> ECSEngine::registerSystem(ERROR &errCode) {
  m_systemManager->registerSystem<T>(errCode);
}

template <typename T> void ECSEngine::setSystemSignature(Signature signature, ERROR &errCode) {
  m_systemManager->setSignature(errCode);
}
