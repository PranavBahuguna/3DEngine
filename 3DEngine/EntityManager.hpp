#pragma once

#include "Error.h"

#include <array>
#include <bitset>
#include <cstdint>
#include <queue>

using Entity = std::uint32_t;
constexpr Entity INVALID_ENTITY = UINT32_MAX;
constexpr Entity MAX_ENTITIES = 10;

using ComponentType = std::uint8_t;
constexpr ComponentType INVALID_COMPONENT_TYPE = UINT8_MAX;
constexpr ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
  EntityManager() : m_nLivingEntities(0) {
    // Initialise all possible entity IDs
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
      m_availableEntities.push(entity);
  }

  Entity create(ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if we have hit the entity limit
    if (m_nLivingEntities >= MAX_ENTITIES) {
      errCode = printErrorMsg(ERROR_ENTITY_LIMIT_REACHED, MAX_ENTITIES);
      return INVALID_ENTITY;
    }

    // Take an ID from the front of the ID queue
    Entity id = m_availableEntities.front();
    m_availableEntities.pop();
    ++m_nLivingEntities;

    return id;
  }

  void destroy(Entity entity, ERROR &errCode) {
    errCode = ERROR_OK;

    // Check we have a valid entity
    if (entity >= MAX_ENTITIES) {
      errCode = printErrorMsg(ERROR_INVALID_ENTITY, entity);
      return;
    }

    // Invalidate the destroyed entity id, and push it to the back of the queue
    m_signatures[entity].reset();
    m_availableEntities.push(entity);
    --m_nLivingEntities;
  }

  void setSignature(Entity entity, Signature signature, ERROR &errCode) {
    // Check we have a valid entity
    if (entity >= MAX_ENTITIES) {
      errCode = printErrorMsg(ERROR_INVALID_ENTITY, entity);
      return;
    }

    m_signatures[entity] = signature;
  }

  Signature getSignature(Entity entity, ERROR &errCode) {
    // Check we have a valid entity
    if (entity >= MAX_ENTITIES) {
      errCode = printErrorMsg(ERROR_INVALID_ENTITY, entity);
      return INVALID_ENTITY;
    }

    return m_signatures[entity];
  }

private:
  std::queue<Entity> m_availableEntities;
  std::array<Signature, MAX_ENTITIES> m_signatures;
  uint32_t m_nLivingEntities;
};