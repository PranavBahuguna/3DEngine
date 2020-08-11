#include "GameObject.h"

#include <stdexcept>

GameObject::GameObject(ModelSptr model) : GameObject(model, "") {}

GameObject::GameObject(ModelSptr model, const std::string &script)
    : m_model(model), m_script(ResourceManager<Script>::Get(script, *this)) {
  ERROR errCode = ERROR_OK;
  model->load(errCode);
  if (errCode == ERROR_OK)
    init(errCode);

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while creating GameObject.");
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
