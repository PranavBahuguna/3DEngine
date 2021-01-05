#pragma once

#include "Component.h"
#include "Resource.h"

#define SOL_ALL_SAFETIES_ON 1
#define SOL_NO_EXCEPTIONS 1
#include "sol.hpp"

class Script : public Resource {
public:
  Script(const std::string &name);

  void init(ERROR &errCode);
  void update(ERROR &errCode);

private:
  ERROR load(const std::string &filename);
  void callFunc(ERROR &errCode, const std::string &funcName);
};