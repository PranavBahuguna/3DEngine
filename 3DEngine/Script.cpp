#include "Script.h"
#include "BasicDefines.h"
#include "Timer.h"
#include "Transform.h"

#include <filesystem>
#include <stdexcept>

Script::Script(const std::string &name) : Script(nullptr, name) {}

Script::Script(std::shared_ptr<GameObject> owner, const std::string &name)
    : Component(owner), Resource{name} {
  lua.open_libraries(sol::lib::base, sol::lib::math);

  const std::string filename = "Scripts/" + name + ".lua";
  if (load(filename) != ERROR_OK)
    throw std::runtime_error("An error occurred while loading script (" + filename + ").");

  // Add userdata types
  auto vec3Type = lua.new_usertype<glm::vec3>(
      "vec3", sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(), "x",
      &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
  auto transformType = lua.new_usertype<Transform>(
      "Transform", sol::constructors<Transform(glm::vec3, glm::vec3, glm::vec3)>());

  transformType["setPosition"] = &Transform::setPosition;
  transformType["translate"] = &Transform::translate;
  transformType["setRotation"] = &Transform::setRotation;
  transformType["rotate"] = &Transform::rotate;
  transformType["setScale"] = &Transform::setScale;
  transformType["reScale"] = &Transform::reScale;
  transformType["getPosition"] = &Transform::getPosition;
  transformType["getRotation"] = &Transform::getRotation;
  transformType["getScale"] = &Transform::getScale;
  transformType["getOrientation"] = &Transform::getOrientation;
  transformType["getModel"] = &Transform::getModel;
  transformType["getView"] = &Transform::getView;
  transformType["getFront"] = &Transform::getFront;
  transformType["getRight"] = &Transform::getRight;
  transformType["getUp"] = &Transform::getUp;

  // GameObject functions
  lua.set_function("getTransform", &GameObject::GetComponentNoPtr<Transform>, *m_owner);

  // Math functions
  lua.set_function("deg2rad", [](float angleDegrees) { return DEG2RAD(angleDegrees); });
  lua.set_function("rad2deg", [](float angleRadians) { return RAD2DEG(angleRadians); });
  lua.set_function("sin", [](float angle) { return sin(DEG2RAD(angle)); });
  lua.set_function("cos", [](float angle) { return cos(DEG2RAD(angle)); });

  // Timer
  lua.set_function("getDeltaTime", &Timer::GetDeltaTime);
}

// Load a Lua script from filename
ERROR Script::load(const std::string &filename) {
  // Check if file exists
  if (!std::filesystem::exists(filename))
    return printErrorMsg(ERROR_FILE_LOAD_FAILED, filename.c_str());

  // Try loading the file
  auto result = lua.script_file(filename);
  if (!result.valid()) {
    sol::error err = result;
    return printErrorMsg(ERROR_LUA_ERROR, err.what());
  }

  return ERROR_OK;
}

void Script::init(ERROR &errCode) { callFunc(errCode, "init"); }

void Script::update(ERROR &errCode) { callFunc(errCode, "update"); }

// Calls a lua script function with error handling
void Script::callFunc(ERROR &errCode, const std::string &funcName) {
  sol::protected_function f = lua[funcName];
  if (!f.valid())
    return; // function not defined, ignore

  // Set the error handler
  f.error_handler = lua["error_handler"];

  // Call the function and handle any errors
  auto result = f();
  if (!result.valid()) {
    sol::error err = result;
    errCode = printErrorMsg(ERROR_LUA_ERROR, err.what());
  }
}