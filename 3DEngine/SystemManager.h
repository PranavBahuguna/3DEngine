#pragma once

#include "EntityManager.h"

#include <memory>
#include <set>
#include <unordered_map>

struct System {
  std::set<Entity> entities;
};

class SystemManager {
public:
  template <typename T> std::shared_ptr<T> registerSystem(ERROR &errCode);
  template <typename T> void setSignature(Signature signature, ERROR &errCode);
  void entityDestroyed(Entity entity);
  void entitySignatureChanged(Entity entity, Signature newSignature);

private:
  std::unordered_map<const char *, Signature> m_signatures;
  std::unordered_map<const char *, std::shared_ptr<System>> m_systems;
};