#include "EntityManager.h"

EntityManager::EntityManager() : m_nLivingEntities(0) {
  // Initialise all possible entity IDs
  for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    m_availableEntities.push(entity);
}

Entity EntityManager::create(ERROR &errCode) {
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

void EntityManager::destroy(Entity entity, ERROR &errCode) {
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

void EntityManager::setSignature(Entity entity, Signature id, ERROR &errCode) {
  // Check we have a valid entity
  if (entity >= MAX_ENTITIES) {
    errCode = printErrorMsg(ERROR_INVALID_ENTITY, entity);
    return;
  }

  m_signatures[entity] = id;
}

Signature EntityManager::getSignature(Entity entity, ERROR &errCode) {
  // Check we have a valid entity
  if (entity >= MAX_ENTITIES) {
    errCode = printErrorMsg(ERROR_INVALID_ENTITY, entity);
    return INVALID_ENTITY;
  }

  return m_signatures[entity];
}