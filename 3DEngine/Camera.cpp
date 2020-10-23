#include "Camera.h"
#include "Game.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Transform &transform, float fovy, float zNear, float zFar)
    : m_transform(transform), m_fovy(fovy), m_zNear(zNear), m_zFar(zFar), m_recalcView(true),
      m_recalcProjection(true) {

  updateDirection();
  updateProjection();
}

Transform Camera::getTransform() const { return m_transform; }

glm::mat4 Camera::getView() const { return m_view; }

glm::mat4 Camera::getProjection() const { return m_projection; }

glm::vec3 Camera::getPosition() const { return m_transform.getPosition(); }

float Camera::getPitch() const { return m_transform.getRotation().x; }

float Camera::getYaw() const { return m_transform.getRotation().y; }

float Camera::getFOV() const { return m_fovy; }

float Camera::getZNear() const { return m_zNear; }

float Camera::getZFar() const { return m_zFar; }

void Camera::performAction(CameraAction action, float amount) {
  // Decide whether the action will require recalculation of the view or projection matrices
  switch (action) {
  case CameraAction::MoveFront:
  case CameraAction::MoveRight:
  case CameraAction::MoveUp:
  case CameraAction::TurnRight:
  case CameraAction::TurnUp:
    m_recalcView = true;
    break;
  case CameraAction::Zoom:
    m_recalcProjection = true;
    break;
  }

  switch (action) {
  case CameraAction::MoveFront:
    m_transform.translate(m_front * amount);
    break;
  case CameraAction::MoveRight:
    m_transform.translate(m_right * amount);
    break;
  case CameraAction::MoveUp:
    m_transform.translate(m_up * amount);
    break;
  case CameraAction::TurnRight:
    m_transform.rotate(glm::vec3(0.0f, amount, 0.0f));
    break;
  case CameraAction::TurnUp:
    m_transform.rotate(glm::vec3(amount, 0.0f, 0.0f));
    break;
  case CameraAction::Zoom:
    m_fovy += amount;
    break;
  }
}

void Camera::update() {
  updateDirection();
  updateProjection();
}

// Update the camera's direction
void Camera::updateDirection() {
  if (!m_recalcView)
    return;

  m_transform.limitRotation();

  // Calculate orientation from pitch and yaw
  glm::quat qPitch = glm::angleAxis(-m_transform.getRotation().x, glm::vec3(1, 0, 0));
  glm::quat qYaw = glm::angleAxis(m_transform.getRotation().y, glm::vec3(0, 1, 0));
  m_orientation = qPitch * qYaw;

  // Calculate view matrix
  m_view = glm::mat4_cast(m_orientation);                      // rotation
  m_view = glm::translate(m_view, -m_transform.getPosition()); // translation

  // Calculate the front, right and up vectors
  glm::quat invOrientation = glm::conjugate(m_orientation);
  m_front = glm::rotate(invOrientation, glm::vec3(0, 0, -1));
  m_right = glm::rotate(invOrientation, glm::vec3(1, 0, 0));
  m_up = glm::rotate(invOrientation, glm::vec3(0, 1, 0));

  m_recalcView = false;
}

// Update the camera's projection
void Camera::updateProjection() {
  if (!m_recalcProjection)
    return;

  // Clamp FOV between min and max values
  if (m_fovy < CAMERA_MIN_FOV)
    m_fovy = CAMERA_MIN_FOV;
  if (m_fovy > CAMERA_MAX_FOV)
    m_fovy = CAMERA_MAX_FOV;

  m_projection = glm::perspective(m_fovy, Game::GetWindow().getAspectRatio(), m_zNear, m_zFar);
  m_recalcProjection = false;
}
