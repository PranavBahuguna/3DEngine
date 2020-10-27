#include "Transform.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
    : m_position(position), m_rotation(rotation), m_scale(scale), m_model(glm::mat4()),
      m_positionUpdated(true), m_rotationUpdated(true), m_scaleUpdated(true) {}

Transform &Transform::setPosition(const glm::vec3 &position) {
  if (m_position != position) {
    m_position = position;
    m_positionUpdated = true;
  }
  return *this;
}

Transform &Transform::translate(const glm::vec3 &translation) {
  if (translation != glm::vec3()) {
    m_position += translation;
    m_positionUpdated = true;
  }
  return *this;
}

Transform &Transform::setRotation(const glm::vec3 &rotation) {
  if (m_rotation != rotation) {
    m_rotation = rotation;
    m_rotationUpdated = true;
  }
  return *this;
}

Transform &Transform::rotate(const glm::vec3 &rotation) {
  if (rotation != glm::vec3()) {
    m_rotation += rotation;
    m_rotationUpdated = true;
  }
  return *this;
}

Transform &Transform::setScale(const glm::vec3 &scale) {
  if (m_scale != scale) {
    m_scale = scale;
    m_scaleUpdated = true;
  }
  return *this;
}

Transform &Transform::reScale(const glm::vec3 &scale) {
  if (scale != glm::vec3()) {
    m_scale += scale;
    m_scaleUpdated = true;
  }
  return *this;
}

const glm::vec3 &Transform::getPosition() { return m_position; }

const glm::vec3 &Transform::getRotation() {
  limitRotation();
  return m_rotation;
}

const glm::vec3 &Transform::getScale() { return m_scale; }

const glm::mat4 &Transform::getModel() {
  updateModel();
  return m_model;
}

// Resets rotation angle to an equivalent value between +/- pi radians
void Transform::limitRotation() {
  if (!m_rotationUpdated)
    return;

  constexpr auto limitAngle = [](float &angle) {
    constexpr float pi = glm::pi<float>();

    if (angle > pi || angle < -pi) {
      float adjustValue = (angle >= 0.0f) ? pi : -pi;
      angle = fmodf(angle, pi) - adjustValue;
    }
  };

  limitAngle(m_rotation.x);
  limitAngle(m_rotation.y);
  limitAngle(m_rotation.z);
}

void Transform::updateModel() {
  if (!m_positionUpdated && !m_rotationUpdated && !m_scaleUpdated)
    return;

  m_model = glm::translate(m_model, m_position);
  m_model = glm::rotate(m_model, m_rotation.x, {1.0f, 0.0f, 0.0f});
  m_model = glm::rotate(m_model, m_rotation.y, {0.0f, 1.0f, 0.0f});
  m_model = glm::rotate(m_model, m_rotation.z, {0.0f, 0.0f, 1.0f});
  m_model = glm::scale(m_model, m_scale);

  // Reset transform update flags
  m_positionUpdated = false;
  m_rotationUpdated = false;
  m_scaleUpdated = false;
}