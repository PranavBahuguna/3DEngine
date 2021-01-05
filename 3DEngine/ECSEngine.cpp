#include "ECSEngine.hpp"

std::unique_ptr<EntityManager> ECSEngine::m_entityManager = std::make_unique<EntityManager>();
std::unique_ptr<ComponentManager> ECSEngine::m_componentManager =
    std::make_unique<ComponentManager>();
std::unique_ptr<SystemManager> ECSEngine::m_systemManager = std::make_unique<SystemManager>();