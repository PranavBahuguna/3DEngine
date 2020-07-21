#include "GameObject.h"

#include <stdexcept>

GameObject::GameObject(const std::string &name) : _name(name) {}

// Loads and attaches the Lua script to this
void GameObject::loadScript(const std::string &name, ERROR &errCode) {
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
