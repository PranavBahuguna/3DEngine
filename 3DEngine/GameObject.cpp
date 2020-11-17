#include "GameObject.h"
#include "BasicDefines.h"

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

const glm::vec3 &GameObject::getPos() const { return m_model->transform().getPosition(); }

const glm::vec3 &GameObject::getEuler() const {
  return DEG2RAD(m_model->transform().getRotation());
}

const glm::vec3 &GameObject::getScale() const { return m_model->transform().getScale(); }

void GameObject::setPos(const glm::vec3 &pos) { m_model->transform().setPosition(pos); }

void GameObject::setEuler(const glm::vec3 &euler) {
  m_model->transform().setRotation(DEG2RAD(euler));
}

void GameObject::setScale(const glm::vec3 &scale) { m_model->transform().setScale(scale); }

void GameObject::move(const glm::vec3 &moveVec) { m_model->transform().translate(moveVec); }

void GameObject::rotate(const glm::vec3 &euler) { m_model->transform().rotate(DEG2RAD(euler)); }

void GameObject::scale(const glm::vec3 &scale) { m_model->transform().reScale(scale); }

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
