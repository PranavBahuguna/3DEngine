#include "Projection.h"

#include <glm/gtc/matrix_transform.hpp>

Projection::Projection(float fovy, float aspectRatio, float zNear, float zFar)
    : m_fovy(fovy), m_aspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar),
      m_projection(glm::mat4()), m_projectionUpdated(true) {}

Projection &Projection::setFOV(float fovy) {
  if (m_fovy != fovy) {
    m_fovy = fovy;
    m_projectionUpdated = true;
  }
  return *this;
}

Projection &Projection::zoom(float zoomAmount) {
  if (zoomAmount != 0.0f) {
    m_fovy += zoomAmount;
    m_projectionUpdated = true;
  }
  return *this;
}

Projection &Projection::setAspectRatio(float aspectRatio) {
  if (m_aspectRatio != aspectRatio) {
    m_aspectRatio = aspectRatio;
    m_projectionUpdated = true;
  }
  return *this;
}

Projection &Projection::setNearPlane(float zNear) {
  if (m_zNear != zNear) {
    m_zNear = zNear;
    m_projectionUpdated = true;
  }
  return *this;
}

Projection &Projection::setFarPlane(float zFar) {
  if (m_zFar != zFar) {
    m_zFar = zFar;
    m_projectionUpdated = true;
  }
  return *this;
}

float Projection::getFOV() const { return m_fovy; }

float Projection::getAspectRatio() const { return m_aspectRatio; }

float Projection::getNearPlane() const { return m_zNear; }

float Projection::getFarPlane() const { return m_zFar; }

const glm::mat4 &Projection::getProjection() const {
  updateProjection();
  return m_projection;
}

// Calculates the new projection matrix if it has been updated
void Projection::updateProjection() const {
  if (!m_projectionUpdated)
    return;

  m_projection = glm::perspective(m_fovy, m_aspectRatio, m_zNear, m_zFar);
  m_projectionUpdated = false;
}