#include "Projection.h"

#include <glm/gtc/matrix_transform.hpp>

Projection::Projection(float fovy, float aspectRatio, float zNear, float zFar)
    : m_fovy(fovy), m_aspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar),
      m_projection(glm::mat4()), m_projectionMatrixUpdated(true) {}

Projection &Projection::setFOV(float fovy) {
  if (m_fovy != fovy) {
    m_fovy = fovy;
    m_projectionMatrixUpdated = true;
  }
  return *this;
}

Projection &Projection::zoom(float zoomAmount) {
  if (zoomAmount != 0.0f) {
    m_fovy += zoomAmount;
    m_projectionMatrixUpdated = true;
  }
  return *this;
}

Projection &Projection::setAspectRatio(float aspectRatio) {
  if (m_aspectRatio != aspectRatio) {
    m_aspectRatio = aspectRatio;
    m_projectionMatrixUpdated = true;
  }
  return *this;
}

Projection &Projection::setNearPlane(float zNear) {
  if (m_zNear != zNear) {
    m_zNear = zNear;
    m_projectionMatrixUpdated = true;
  }
  return *this;
}

Projection &Projection::setFarPlane(float zFar) {
  if (m_zFar != zFar) {
    m_zFar = zFar;
    m_projectionMatrixUpdated = true;
  }
  return *this;
}

float Projection::getFOV() { return m_fovy; }

float Projection::getAspectRatio() { return m_aspectRatio; }

float Projection::getNearPlane() { return m_zNear; }

float Projection::getFarPlane() { return m_zFar; }

const glm::mat4 &Projection::getProjection() {
  updateProjectionMatrix();
  return m_projection;
}

void Projection::updateProjectionMatrix() {
  if (!m_projectionMatrixUpdated)
    return;

  m_projection = glm::perspective(m_fovy, m_aspectRatio, m_zNear, m_zFar);
  m_projectionMatrixUpdated = false;
}