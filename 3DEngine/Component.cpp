#include "Component.h"

Component::Component(const std::shared_ptr<GameObject> &owner) : m_owner(owner) {}

Component::Component(const Component &component) : m_owner(nullptr) {}
