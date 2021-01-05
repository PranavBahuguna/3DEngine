#include "ScriptSystem.h"
#include "ECSEngine.hpp"
#include "Script.h"

void ScriptSystem::init(ERROR &errCode) {
  for (const auto &entity : entities) {
    auto &script = ECSEngine::GetComponent<Script>(entity, errCode);
    script.init(errCode);
  }
}

void ScriptSystem::update(ERROR &errCode) {
  for (const auto &entity : entities) {
    auto &script = ECSEngine::GetComponent<Script>(entity, errCode);
    script.update(errCode);
  }
}