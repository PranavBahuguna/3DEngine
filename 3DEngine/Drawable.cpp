#include "Drawable.h"

Drawable::Drawable() : Drawable(nullptr) {}

Drawable::Drawable(std::shared_ptr<GameObject> owner)
    : Component(owner), _id(id++), m_isActive(true) {}

void Drawable::setActive(bool value) { m_isActive = value; }

bool Drawable::isActive() const { return m_isActive; }

Transform &Drawable::transform() { return m_owner->GetComponentNoPtr<Transform>(); }