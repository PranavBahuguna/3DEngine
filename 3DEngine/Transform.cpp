#include "Transform.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
    : Transform(nullptr, position, rotation, scale) {}

Transform::Transform(const std::shared_ptr<GameObject> &owner, const glm::vec3 &position,
                     const glm::vec3 &rotation, const glm::vec3 &scale)
    : Component(owner), m_position(position), m_rotation(rotation), m_scale(scale),
      m_orientation(glm::mat4()), m_model(glm::mat4()), m_view(glm::mat4()),
      m_orientationUpdated(true), m_modelUpdated(true), m_viewUpdated(true) {}

Transform &Transform::setPosition(const glm::vec3 &position) {
  if (m_position != position) {
    m_position = position;
    m_modelUpdated = true;
    m_viewUpdated = true;
  }
  return *this;
}

Transform &Transform::translate(const glm::vec3 &translation) {
  return setPosition(m_position + translation);
}

// Rotate transform to a direction given by pitch, yaw and roll
Transform &Transform::setRotation(const glm::vec3 &rotation) {
  limitRotation(const_cast<glm::vec3 &>(rotation));
  if (m_rotation != rotation) {
    m_rotation = rotation;
    m_orientationUpdated = true;
    m_modelUpdated = true;
    m_viewUpdated = true;
  }
  return *this;
}

Transform &Transform::rotate(const glm::vec3 &rotation) {
  return setRotation(m_rotation + rotation);
}

Transform &Transform::setScale(const glm::vec3 &scale) {
  m_scale = scale;
  m_modelUpdated = true;
  return *this;
}

Transform &Transform::reScale(const glm::vec3 &scale) { return setScale(m_scale + scale); }

const glm::vec3 &Transform::getPosition() const { return m_position; }

const glm::vec3 &Transform::getRotation() const { return m_rotation; }

const glm::vec3 &Transform::getScale() const { return m_scale; }

const glm::quat &Transform::getOrientation() const {
  updateOrientation();
  return m_orientation;
}

const glm::mat4 &Transform::getModel() const {
  updateModel();
  return m_model;
}

const glm::mat4 &Transform::getView() const {
  updateView();
  return m_view;
}

glm::vec3 Transform::getFront() const {
  updateOrientation();
  return glm::rotate(glm::conjugate(m_orientation), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Transform::getRight() const {
  updateOrientation();
  return glm::rotate(glm::conjugate(m_orientation), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::getUp() const {
  updateOrientation();
  return glm::rotate(glm::conjugate(m_orientation), glm::vec3(0.0f, 1.0f, 0.0f));
}

// Produces a rotation vector with all angles limited between +/- pi radians
void Transform::limitRotation(glm::vec3 &rotation) const {
  constexpr auto limitAngle = [](float &angle) {
    constexpr float pi = glm::pi<float>();

    if (angle > pi || angle < -pi) {
      float adjustValue = (angle >= 0.0f) ? pi : -pi;
      angle = fmodf(angle, pi) - adjustValue;
    }
  };

  limitAngle(rotation.x);
  limitAngle(rotation.y);
  limitAngle(rotation.z);
}

// Recalculates orientation quaternion if it has been updated
void Transform::updateOrientation() const {
  if (!m_orientationUpdated)
    return;

  // Calculate orientation from rotation pitch, yaw and roll
  glm::quat qPitch = glm::angleAxis(-m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat qYaw = glm::angleAxis(m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat qRoll = glm::angleAxis(m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  m_orientation = qPitch * qYaw * qRoll;

  // Reset orientation update flag
  m_orientationUpdated = false;
}

// Recalculates the model matrix if it has been updated
void Transform::updateModel() const {
  if (!m_modelUpdated)
    return;

  updateOrientation();

  // Calculate model from position, orientation and scale
  m_model = glm::translate(glm::mat4(1.0f), m_position);
  m_model *= glm::mat4_cast(m_orientation);
  m_model = glm::scale(m_model, m_scale);

  // Reset model update flag
  m_modelUpdated = false;
}

// Recalculates the view matrix if it has been updated
void Transform::updateView() const {
  if (!m_viewUpdated)
    return;

  updateOrientation();
  m_view = glm::mat4_cast(m_orientation);       // rotation
  m_view = glm::translate(m_view, -m_position); // translation

  // Reset view update flag
  m_viewUpdated = false;
}