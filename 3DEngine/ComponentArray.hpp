#pragma once

#include "IComponentArray.hpp"

#include <unordered_map>

template <typename T> class ComponentArray : public IComponentArray {
public:
  ComponentArray() : m_size(0) {}

  void add(Entity entity, T &component, ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if component is already contained in this array
    if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()) {
      errCode = printErrorMsg(ERROR_ENTITY_ALREADY_HAS_COMPONENT, entity, typeid(T).name());
      return;
    }

    // Append new component entry at end and update the maps
    size_t newIndex = m_size;
    m_entityToIndexMap[entity] = newIndex;
    m_indexToEntityMap[newIndex] = entity;
    m_componentArray[newIndex] = component;
    ++m_size;
  }

  void remove(Entity entity, ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if component exists in this array
    if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()) {
      errCode = printErrorMsg(ERROR_COMPONENT_NOT_FOUND, typeid(T).name());
      return;
    }

    // Copy the last element into the removed element's place and update maps to point at this new
    // location. This ensures the array remains contiguous and dense.
    size_t removedEntityIndex = m_entityToIndexMap[entity];
    size_t lastEntityIndex = m_size - 1;
    Entity lastEntity = m_indexToEntityMap[lastEntityIndex];

    m_componentArray[removedEntityIndex] = m_componentArray[lastEntityIndex];
    m_entityToIndexMap[lastEntity] = removedEntityIndex;
    m_indexToEntityMap[removedEntityIndex] = lastEntity;

    // Remove the deleted and last elements from their respective maps
    m_entityToIndexMap.erase(entity);
    m_indexToEntityMap.erase(lastEntityIndex);
    --m_size;
  }

  T &get(Entity entity, ERROR &errCode) {
    errCode = ERROR_OK;

    // Check if component exists in this array
    const auto &entry = m_entityToIndexMap.find(entity);
    if (entry != m_entityToIndexMap.end())
      errCode = printErrorMsg(ERROR_COMPONENT_NOT_FOUND, typeid(T).name());

    return m_componentArray[entry->second];
  }

  void entityDestroyed(Entity entity, ERROR &errCode) override {
    // If the entity has this component, then remove it
    if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
      remove(entity, errCode);
  }

private:
  std::array<T, MAX_ENTITIES> m_componentArray;
  std::unordered_map<Entity, size_t> m_entityToIndexMap;
  std::unordered_map<size_t, Entity> m_indexToEntityMap;
  size_t m_size;
};