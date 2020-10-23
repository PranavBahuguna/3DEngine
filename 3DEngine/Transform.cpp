#include "Transform.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

glm::mat4 Transform::getModel() const {
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, m_rotation.x, {1.0f, 0.0f, 0.0f});
  model = glm::rotate(model, m_rotation.y, {0.0f, 1.0f, 0.0f});
  model = glm::rotate(model, m_rotation.z, {0.0f, 0.0f, 1.0f});
  model = glm::scale(model, m_scale);

  return model;
}

// Resets rotation angle to an equivalent value between +/- pi radians
void Transform::limitRotation() {
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