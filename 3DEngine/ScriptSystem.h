#pragma once

#include "Script.h"
#include "SystemManager.hpp"

class ScriptSystem : public System {
public:
  void init(ERROR &errCode) override;
  void update(ERROR &errCode) override;

private:
  void callFunc(Script &script, const std::string &funcName, ERROR &errCode);
  void load(Script &script, ERROR &errCode);
};