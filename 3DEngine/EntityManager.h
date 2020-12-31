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
  EntityManager();

  Entity create(ERROR &errCode);
  void destroy(Entity entity, ERROR &errCode);
  void setSignature(Entity entity, Signature signature, ERROR &errCode);
  Signature getSignature(Entity entity, ERROR &errCode);

private:
  std::queue<Entity> m_availableEntities;
  std::array<Signature, MAX_ENTITIES> m_signatures;
  uint32_t m_nLivingEntities;
};