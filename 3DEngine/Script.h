#pragma once

#include "Component.h"
#include "Resource.h"

#define SOL_ALL_SAFETIES_ON 1
#define SOL_NO_EXCEPTIONS 1
#include "sol.hpp"

// Forward declarations
class GameObject;

class Script : public Component, Resource {
public:
  Script(const std::string &name);
  Script(std::shared_ptr<GameObject> owner, const std::string &name);

  void init(ERROR &errCode) override;
  void update(ERROR &errCode) override;

private:
  sol::state lua;

  ERROR load(const std::string &filename);
  void callFunc(ERROR &errCode, const std::string &funcName);
};