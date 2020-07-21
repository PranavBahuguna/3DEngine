#include "Script.h"
#include "Timer.h"

#include <filesystem>
#include <stdexcept>

static sol::state lua;

Script::Script(const std::string &filename, const GameObject &gameObject) {
  ERROR errCode = ERROR_OK;
  lua.open_libraries(sol::lib::base);

  load(filename, errCode);
  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while loading script (" + filename + ").");

  // Register all required functions
  lua.set_function("getAngle", &GameObject::getAngle, gameObject);
  lua.set_function("setAngle", &GameObject::setAngle, gameObject);
  lua.set_function("getDeltaTime", &Timer::GetDeltaTime);
}

// Load a Lua script from filename
void Script::load(const std::string &filename, ERROR &errCode) {
  // Check if file exists
  if (!std::filesystem::exists(filename)) {
    errCode = ERROR_FILE_LOAD_FAILED;
    printErrorMsg(errCode, filename.c_str());
    return;
  }

  // Try loading the file
  auto result = lua.script_file(filename);
  if (!result.valid())
    luaError(errCode, result);
}

// Calls a lua script function with error handling
void Script::callFunc(const std::string &funcName, ERROR &errCode) {
  sol::protected_function f = lua[funcName];
  if (!f.valid())
    return; // function not defined, ignore

  // Set the error handler
  f.error_handler = lua["error_handler"];

  // Call the function and handle any errors
  auto result = f();
  if (!result.valid())
    luaError(errCode, result);
}

// Handles Lua errors from script
void Script::luaError(ERROR &errCode, const sol::error &solErr) const {
  errCode = ERROR_LUA_ERROR;
  printErrorMsg(errCode, solErr.what());
}