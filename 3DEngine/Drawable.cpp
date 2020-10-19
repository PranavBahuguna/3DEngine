#include "Drawable.h"

Drawable::Drawable(const glm::vec3 &pos) : _id(id++), m_pos(pos), m_isActive(true) {}

void Drawable::setActive(bool value) { m_isActive = value; }

bool Drawable::isActive() const { return m_isActive; }

glm::vec3 Drawable::getPos() const { return m_pos; }