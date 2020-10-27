#include "View.h"

View::View(const Transform &transform)
    : m_transform(transform), m_viewMatrix(glm::mat4()), m_orientation(glm::quat()) {}

View &View::setPosition(const glm::vec3 &position) {
  m_transform.setPosition(position);
  return *this;
}

View &View::translate(const glm::vec3 &translation) {
  m_transform.translate(translation);
  return *this;
}

View &View::setRotation(const glm::vec3 &rotation) {
  m_transform.setRotation(rotation);
  return *this;
}

View &View::rotate(const glm::vec3 &rotation) {
  m_transform.rotate(rotation);
  return *this;
}

const glm::mat4 &View::getView() {
  updateView();
  return m_viewMatrix;
}

const glm::quat &View::getOrientation() {
  updateOrientation();
  return m_orientation;
}

const glm::vec3 &View::getPosition() { return m_transform.getPosition(); }

const glm::vec3 &View::getRotation() { return m_transform.getRotation(); }

glm::vec3 View::getFront() {
  updateOrientation();
  const glm::quat invOrientation = glm::conjugate(m_orientation);
  return glm::rotate(invOrientation, glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 View::getRight() {
  updateOrientation();
  const glm::quat invOrientation = glm::conjugate(m_orientation);
  return glm::rotate(invOrientation, glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 View::getUp() {
  updateOrientation();
  const glm::quat invOrientation = glm::conjugate(m_orientation);
  return glm::rotate(invOrientation, glm::vec3(0.0f, 1.0f, 0.0f));
}

void View::updateView() {
  if (!m_transform.m_positionUpdated && !m_transform.m_rotationUpdated)
    return;

  updateOrientation();

  // Calculate view matrix
  m_viewMatrix = glm::mat4_cast(m_orientation);                            // rotation
  m_viewMatrix = glm::translate(m_viewMatrix, -m_transform.getPosition()); // translation
}

void View::updateOrientation() {
  if (!m_transform.m_rotationUpdated)
    return;

  // Calculate orientation from pitch and yaw
  glm::quat qPitch = glm::angleAxis(-m_transform.getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat qYaw = glm::angleAxis(m_transform.getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
  m_orientation = qPitch * qYaw;
}