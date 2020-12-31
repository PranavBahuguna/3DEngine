#include "ComponentManager.h"

ComponentManager::ComponentManager() : m_nextComponentType(0) {}

template <typename T> void ComponentManager::registerComponent(ERROR &errCode) {
  errCode = ERROR_OK;

  // Check if component is already registered
  const char *typeName = typeid(T).name();
  if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
    errCode = printErrorMsg(ERROR_COMPONENT_ALREADY_REGISTERED, typeName);
    return;
  }

  // Add this to the component types map and create a new component array for this component type.
  // Increment the component type index to give the next registered type a new index.
  m_componentTypes[typeName] = m_nextComponentType++;
  m_componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
}

template <typename T> ComponentType ComponentManager::getComponentType(ERROR &errCode) {
  errCode = ERROR_OK;

  // Check if component is registered
  const char *typeName = typeid(T).name();
  if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
    errCode = printErrorMsg(ERROR_COMPONENT_NOT_REGISTERED, typeName);
    return INVALID_COMPONENT_TYPE;
  }

  return m_componentTypes[typeName];
}

// Adds a component to the array for a specific entity
template <typename T>
void ComponentManager::addComponent(Entity entity, T &component, ERROR &errCode) {
  errCode = ERROR_OK;

  auto &componentArray = getComponentArray<T>(errCode);
  if (errCode == ERROR_OK)
    componentArray->add(entity, component, errCode);
}

// Removes a component from the array for a specific entity
template <typename T> void ComponentManager::removeComponent(Entity entity, ERROR &errCode) {
  errCode = ERROR_OK;

  auto &componentArray = getComponentArray<T>(errCode);
  if (errCode == ERROR_OK)
    componentArray->remove(entity, errCode);
}

// Get a reference to a component from the array for an entity
template <typename T> T &ComponentManager::getComponent(Entity entity, ERROR &errCode) {
  errCode = ERROR_OK;

  auto &componentArray = getComponentArray<T>(errCode);
  if (errCode == ERROR_OK)
    componentArray->get(entity, errCode);
}

void ComponentManager::entityDestroyed(Entity entity) {
  // Notify each component array that an entity has been destroyed. If the array has a component for
  // that entity, the entity will be removed.
  std::for_each(m_componentArrays.begin(), m_componentArrays.end(),
                [entity](const auto &pair) { pair.second->entityDestroyed(entity); });
}

template <typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::getComponentArray(ERROR &errCode) {
  errCode = ERROR_OK;

  // Check if component is registered
  const char *typeName = typeid(T).name();
  if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
    errCode = printErrorMsg(ERROR_COMPONENT_NOT_REGISTERED, typeName);
    return nullptr;
  }

  return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
}