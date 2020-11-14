#include "Drawable.h"

Drawable::Drawable(const Transform &transform)
    : _id(id++), m_transform(transform), m_isActive(true) {}

void Drawable::setActive(bool value) { m_isActive = value; }

bool Drawable::isActive() const { return m_isActive; }

Transform &Drawable::transform() { return m_transform; }