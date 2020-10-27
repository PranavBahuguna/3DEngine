#include "Camera.h"
#include "Game.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const View &view, float fovy, float zNear, float zFar)
    : m_view(view), m_fovy(fovy), m_zNear(zNear), m_zFar(zFar), m_recalcProjection(true) {

  updateProjection();
}

glm::mat4 Camera::getView() const { return m_view.getView(); }

glm::mat4 Camera::getProjection() const { return m_projection; }

glm::vec3 Camera::getPosition() const { return m_view.getPosition(); }

float Camera::getPitch() const { return m_view.getRotation().x; }

float Camera::getYaw() const { return m_view.getRotation().y; }

float Camera::getFOV() const { return m_fovy; }

float Camera::getZNear() const { return m_zNear; }

float Camera::getZFar() const { return m_zFar; }

void Camera::performAction(CameraAction action, float amount) {
  switch (action) {
  case CameraAction::MoveFront:
    m_view.translate(m_view.getFront() * amount);
    break;
  case CameraAction::MoveRight:
    m_view.translate(m_view.getRight() * amount);
    break;
  case CameraAction::MoveUp:
    m_view.translate(m_view.getUp() * amount);
    break;
  case CameraAction::TurnRight:
    m_view.rotate(glm::vec3(0.0f, amount, 0.0f));
    break;
  case CameraAction::TurnUp:
    m_view.rotate(glm::vec3(amount, 0.0f, 0.0f));
    break;
  case CameraAction::Zoom:
    m_fovy += amount;
    break;
  }
}

void Camera::update() { updateProjection(); }

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
