#include "ScriptSystem.h"
#include "BasicDefines.h"
#include "ECSEngine.hpp"
#include "Lua.hpp"
#include "Script.h"
#include "Timer.h"
#include "Transform.h"

#include <filesystem>

void ScriptSystem::init(ERROR &errCode) {
  for (const auto &entity : entities) {
    auto &script = ECSEngine::GetComponent<Script>(entity, errCode);
    load(script, errCode);

    // Add userdata types
    auto vec3Type = script.lua->new_usertype<glm::vec3>(
        "vec3", sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(), "x",
        &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
    auto transformType = script.lua->new_usertype<Transform>(
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

    // Math functions
    script.lua->set_function("deg2rad", [](float angleDegrees) { return DEG2RAD(angleDegrees); });
    script.lua->set_function("rad2deg", [](float angleRadians) { return RAD2DEG(angleRadians); });
    script.lua->set_function("sin", [](float angle) { return sin(DEG2RAD(angle)); });
    script.lua->set_function("cos", [](float angle) { return cos(DEG2RAD(angle)); });

    // Timer
    script.lua->set_function("getDeltaTime", &Timer::GetDeltaTime);

    // Entity-specific functions
    script.lua->set_function("getTransform", [entity, &errCode]() {
      return ECSEngine::GetComponent<Transform>(entity, errCode);
    });

    callFunc(script, "init", errCode);
  }
}

void ScriptSystem::update(ERROR &errCode) {
  for (const auto &entity : entities) {
    auto &script = ECSEngine::GetComponent<Script>(entity, errCode);
    callFunc(script, "update", errCode);
  }
}

// Calls a lua script function with error handling
void ScriptSystem::callFunc(Script &script, const std::string &funcName, ERROR &errCode) {
  sol::protected_function f = (*script.lua)[funcName];
  if (!f.valid())
    return; // function not defined, ignore

  // Set the error handler
  f.error_handler = (*script.lua)["error_handler"];

  // Call the function and handle any errors
  auto result = f();
  if (!result.valid()) {
    sol::error err = result;
    errCode = printErrorMsg(ERROR_LUA_ERROR, err.what());
  }
}

// Load the Lua script
void ScriptSystem::load(Script &script, ERROR &errCode) {
  // Check if file exists
  if (!std::filesystem::exists(script._name)) {
    errCode = printErrorMsg(ERROR_FILE_LOAD_FAILED, script._name.c_str());
    return;
  }

  // Try loading the file
  auto result = script.lua->script_file(script._name);
  if (!result.valid()) {
    sol::error err = result;
    errCode = printErrorMsg(ERROR_LUA_ERROR, err.what());
  }
}