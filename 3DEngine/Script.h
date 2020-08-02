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

  void initFunc(ERROR &errCode) { callFunc(errCode, "init"); }
  void updateFunc(ERROR &errCode) { callFunc(errCode, "update"); }

private:
  sol::state lua;

  void load(ERROR &errCode, const std::string &filename);
  void callFunc(ERROR &errCode, const std::string &funcName);
};