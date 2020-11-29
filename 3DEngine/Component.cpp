#include "Component.h"

Component::Component(std::shared_ptr<GameObject> owner) : m_owner(owner) {}

Component::Component(const Component &component) : m_owner(nullptr) {}