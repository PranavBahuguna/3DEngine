#pragma once

#include "ComponentArray.hpp"
#include "EntityManager.hpp"

#include <memory>
#include <unordered_map>

class ComponentManager {
public:
  ComponentManager() : m_nextComponentType(0) {}

  template <typename T> void registerComponent(ERROR &errCode) {
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

  template <typename T> ComponentType getComponentType(ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if component is registered
    const char *typeName = typeid(T).name();
    if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
      errCode = printErrorMsg(ERROR_COMPONENT_NOT_REGISTERED, typeName);
      return INVALID_COMPONENT_TYPE;
    }

    return m_componentTypes[typeName];
  }

  template <typename T> void addComponent(Entity entity, const T &component, ERROR &errCode) {
    errCode = ERROR_OK;

    auto componentArray = getComponentArray<T>(errCode);
    if (errCode == ERROR_OK)
      componentArray->add(entity, component, errCode);
  }

  template <typename T> void removeComponent(Entity entity, ERROR &errCode) {
    errCode = ERROR_OK;

    auto componentArray = getComponentArray<T>(errCode);
    if (errCode == ERROR_OK)
      componentArray->remove(entity, errCode);
  }

  template <typename T> T &getComponent(Entity entity, ERROR &errCode) {
    return getComponentArray<T>(errCode)->get(entity, errCode);
  }

  void entityDestroyed(Entity entity, ERROR &errCode) {
    // Notify each component array that an entity has been destroyed. If the array has a component
    // for that entity, the entity will be removed.
    for (auto it = m_componentArrays.begin(); it != m_componentArrays.end() && errCode != ERROR_OK;
         ++it)
      it->second->entityDestroyed(entity, errCode);
  }

private:
  template <typename T> std::shared_ptr<ComponentArray<T>> getComponentArray(ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if component is registered
    const char *typeName = typeid(T).name();
    if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
      errCode = printErrorMsg(ERROR_COMPONENT_NOT_REGISTERED, typeName);
      return nullptr;
    }

    return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
  }

  std::unordered_map<const char *, ComponentType> m_componentTypes;
  std::unordered_map<const char *, std::shared_ptr<IComponentArray>> m_componentArrays;
  ComponentType m_nextComponentType;
};