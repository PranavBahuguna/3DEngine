#include "GameObject.h"
#include "Model.h"

void GameObject::init(ERROR &errCode) {
  for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
    m_components[i]->init(errCode);
}

void GameObject::start(ERROR &errCode) {
  for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
    m_components[i]->start(errCode);
}

void GameObject::update(ERROR &errCode) {
  for (size_t i = 0; i < m_components.size() && errCode == ERROR_OK; ++i)
    m_components[i]->update(errCode);
}