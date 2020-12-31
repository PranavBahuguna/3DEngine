#include "SystemManager.h"

template <typename T> std::shared_ptr<T> SystemManager::registerSystem(ERROR &errCode) {
  errCode = ERROR_OK;

  // Check if the system has already been registered
  const char *typeName = typeid(T).name();
  if (m_systems.find(typeName) != m_systems.end()) {
    errCode = printErrorMsg(ERROR_SYSTEM_ALREADY_REGISTERED, typeName);
    return nullptr;
  }

  // Create a new system of the given type and return a pointer to it
  auto system = std::make_shared<T>();
  m_systems[typeName] = system;

  return system;
}

template <typename T> void SystemManager::setSignature(Signature signature, ERROR &errCode) {
  errCode = ERROR_OK;

  // Check if the system is registered
  const char *typeName = typeid(T).name();
  if (m_systems.find(typeName) == m_systems.end()) {
    errCode = printErrorMsg(ERROR_SYSTEM_NOT_REGISTERED, typeName);
    return;
  }

  m_signatures[typeName] = signature;
}

void SystemManager::entityDestroyed(Entity entity) {
  // Remove the entity from all registered systems
  std::for_each(m_systems.begin(), m_systems.end(),
                [entity](const auto &pair) { pair.second->entities.erase(entity); });
}

void SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature) {
  // Notify each system that an entity's signature changed
  for (const auto &[typeName, system] : m_systems) {
    const auto &systemSignature = m_signatures[typeName];

    // Add entity to the system if its signature is contained within the system signature, otherwise
    // (if it exists in the system) remove it
    if ((entitySignature & systemSignature) == systemSignature)
      system->entities.insert(entity);
    else
      system->entities.erase(entity);
  }
}