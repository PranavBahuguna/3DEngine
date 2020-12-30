#pragma once

#include "IComponentArray.h"

#include <unordered_map>

template <typename T> class ComponentArray : public IComponentArray {
public:
  void add(Entity entity, T &component, ERROR &errCode);
  void remove(Entity entity, ERROR &errCode);
  T &get(Entity entity, ERROR &errCode);
  void entityDestroyed(Entity entity) override;

private:
  std::array<T, MAX_ENTITIES> m_componentArray;
  std::unordered_map<Entity, size_t> m_entityToIndexMap;
  std::unordered_map<size_t, Entity> m_indexToEntityMap;
  size_t m_size;
};