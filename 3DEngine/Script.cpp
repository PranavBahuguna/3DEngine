#include "Script.h"
#include "Timer.h"

#include <filesystem>
#include <stdexcept>

Script::Script(const std::string &filename, const GameObject &gameObject) {
  ERROR errCode = ERROR_OK;
  lua.open_libraries(sol::lib::base, sol::lib::math);

  load(filename, errCode);
  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while loading script (" + filename + ").");

  // Add userdata types
  lua.new_usertype<glm::vec3>("vec3",
                              sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
                              "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);

  // GameObject functions
  lua.set_function("getPos", &GameObject::getPos, gameObject);
  lua.set_function("getEuler", &GameObject::getEuler, gameObject);
  lua.set_function("getScale", &GameObject::getScale, gameObject);
  lua.set_function("setPos", &GameObject::setPos, gameObject);
  lua.set_function("setEuler", &GameObject::setEuler, gameObject);
  lua.set_function("setScale", &GameObject::setScale, gameObject);
  lua.set_function("move", &GameObject::move, gameObject);
  lua.set_function("rotate", &GameObject::rotate, gameObject);
  lua.set_function("scale", &GameObject::scale, gameObject);

  // Math functions
  lua.set_function("sin", [](float angle) { return sin(glm::radians(angle)); });
  lua.set_function("cos", [](float angle) { return cos(glm::radians(angle)); });

  // Timer
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