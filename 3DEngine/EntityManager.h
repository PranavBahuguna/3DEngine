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

using EntityId = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
  EntityManager();

  Entity create(ERROR &errCode);
  void destroy(Entity entity, ERROR &errCode);
  void setId(Entity entity, EntityId id, ERROR &errCode);
  EntityId getId(Entity entity, ERROR &errCode = error_placeholder);

private:
  std::queue<Entity> m_availableEntities;
  std::array<EntityId, MAX_ENTITIES> m_ids;
  uint32_t m_nLivingEntities;
};