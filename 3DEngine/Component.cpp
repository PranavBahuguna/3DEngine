#include "Component.h"

Component::Component(std::shared_ptr<GameObject> owner) : m_owner(owner) {}

Component::Component(const Component &component) : m_owner(nullptr) {}

std::shared_ptr<GameObject> Component::getOwner() { return m_owner; }