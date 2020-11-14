#include "Camera.h"
#include "Game.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Transform &transform, const Projection &projection)
    : m_transform(transform), m_projection(projection) {}

Transform &Camera::transform() { return m_transform; }

Projection &Camera::projection() { return m_projection; }

void Camera::performAction(CameraAction action, float amount) {
  switch (action) {
  case CameraAction::MoveFront:
    m_transform.translate(m_transform.getFront() * amount);
    break;
  case CameraAction::MoveRight:
    m_transform.translate(m_transform.getRight() * amount);
    break;
  case CameraAction::MoveUp:
    m_transform.translate(m_transform.getUp() * amount);
    break;
  case CameraAction::TurnRight:
    m_transform.rotate(glm::vec3(0.0f, amount, 0.0f));
    break;
  case CameraAction::TurnUp:
    m_transform.rotate(glm::vec3(amount, 0.0f, 0.0f));
    break;
  case CameraAction::Zoom:
    float newFov = m_projection.zoom(amount).getFOV();

    // Clamp FOV between min and max values
    if (newFov < CAMERA_MIN_FOV)
      m_projection.setFOV(CAMERA_MIN_FOV);
    if (newFov > CAMERA_MAX_FOV)
      m_projection.setFOV(CAMERA_MAX_FOV);
    break;
  }
}