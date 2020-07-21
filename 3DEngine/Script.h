#pragma once

#include "Error.h"
#include "GameObject.h"

#define SOL_ALL_SAFETIES_ON 1
#define SOL_NO_EXCEPTIONS 1
#include "sol.hpp"

// Forward declarations
class GameObject;

class Script {
public:
  Script(const std::string &name, const GameObject &gameObject);

  void initFunc(ERROR &errCode) { callFunc("init", errCode); }
  void updateFunc(ERROR &errCode) { callFunc("update", errCode); }

private:
  void load(const std::string &filename, ERROR &errCode);
  void callFunc(const std::string &funcName, ERROR &errCode);
  void luaError(ERROR &errCode, const sol::error &solErr) const;
};