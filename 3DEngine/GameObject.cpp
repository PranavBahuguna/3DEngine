#include "GameObject.h"
#include "Model.h"

GameObject::GameObject() : m_isActive(true), m_isVisible(true) {}

void GameObject::init(ERROR &errCode) {
  for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
    m_components[i]->init(errCode);
}

void GameObject::start(ERROR &errCode) {
  for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
    m_components[i]->start(errCode);
}

void GameObject::update(ERROR &errCode) {
  if (m_isActive) {
    for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
      m_components[i]->update(errCode);
  }
}

void GameObject::draw(ERROR &errCode, const Shader &shader) {
  if (m_isVisible) {
    for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
      m_components[i]->draw(errCode, shader);
  }
}

void GameObject::setActive(bool value) { m_isActive = value; }

void GameObject::setVisible(bool value) { m_isVisible = value; }

bool GameObject::isActive() const { return m_isActive; }

bool GameObject::isVisible() const { return m_isVisible; }