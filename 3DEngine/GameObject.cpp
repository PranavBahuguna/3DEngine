#include "GameObject.h"

#include <stdexcept>

GameObject::GameObject(ModelPtr model) : GameObject(model, "") {}

GameObject::GameObject(ModelPtr model, const std::string &scriptName) : m_model(model) {
  ERROR errCode = ERROR_OK;

  loadScript(scriptName);
  model->load(errCode);
  if (errCode == ERROR_OK)
    init(errCode);

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while creating GameObject.");
}

// Loads and attaches the Lua script to this
void GameObject::loadScript(const std::string &name) {
  if (name.empty())
    return;

  const std::string filename = "Scripts/" + name + ".lua";
  m_script = ScriptPtr(new Script(filename, *this));
}

// Calls the init function from attached script (if any)
void GameObject::init(ERROR &errCode) {
  if (m_script != nullptr)
    m_script->initFunc(errCode);
}

// Calls the update function from attached script (if any)
void GameObject::update(ERROR &errCode) {
  if (m_script != nullptr)
    m_script->updateFunc(errCode);
}
