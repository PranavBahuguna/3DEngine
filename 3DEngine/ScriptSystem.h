#pragma once

#include "SystemManager.hpp"

class ScriptSystem : public System {
public:
  void init(ERROR &errCode) override;
  void update(ERROR &errCode) override;
};