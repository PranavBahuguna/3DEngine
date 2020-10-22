#include "Transform.h"

Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {}

Transform &Transform::setPosition(const glm::vec3 &position) {
  m_position = position;
  return *this;
}

Transform &Transform::translate(const glm::vec3 &translation) {
  m_position += translation;
  return *this;
}

Transform &Transform::setRotation(const glm::vec3 &rotation) {
  m_rotation = rotation;
  return *this;
}

Transform &Transform::rotate(const glm::vec3 &rotation) {
  m_rotation += rotation;
  return *this;
}

Transform &Transform::setScale(const glm::vec3 &scale) {
  m_scale = scale;
  return *this;
}

Transform &Transform::reScale(const glm::vec3 &scale) {
  m_scale += scale;
  return *this;
}

const glm::vec3 &Transform::getPosition() const { return m_position; }

const glm::vec3 &Transform::getRotation() const { return m_rotation; }

const glm::vec3 &Transform::getScale() const { return m_scale; }