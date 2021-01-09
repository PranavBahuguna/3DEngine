#pragma once

#include "Component.h"
#include "Resource.h"

#define SOL_ALL_SAFETIES_ON 1
#define SOL_NO_EXCEPTIONS 1
#include "sol.hpp"

struct Script : public Resource {
  sol::state *lua;
};