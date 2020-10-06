#include "Projection.h"

#include <glm/gtc/matrix_transform.hpp>

// Default - create empty orthographic projection
Projection::Projection()
    : m_fovy(0.0f), m_aspectRatio(0.0f), m_zNear(0.0f), m_zFar(0.0f),
      m_projectionType(ProjectionType::orthographic), m_recalcProjectionMatrix(false),
      m_projectionMatrix(0) {}

// Orthographic projection constructor
Projection::Projection(float left, float right, float bottom, float top, float zNear, float zFar)
    : m_fovy(0.0f), m_zNear(zNear), m_zFar(zFar), m_projectionType(ProjectionType::orthographic),
      m_recalcProjectionMatrix(false) {

  m_aspectRatio = (right - left) / (top - bottom);
  m_projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
}

// Perspective projection constructor
Projection::Projection(float fovy, float aspectRatio, float zNear, float zFar)
    : m_fovy(fovy), m_aspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar),
      m_projectionType(ProjectionType::perspective), m_recalcProjectionMatrix(false) {

  m_projectionMatrix = glm::perspective(fovy, aspectRatio, zNear, zFar);
}

Projection::~Projection() {}

// Obtains the projection matrix, recalculating it if necessary
glm::mat4 Projection::getProjectionMatrix() const {
  if (m_recalcProjectionMatrix) {
    m_projectionMatrix = glm::perspective(m_fovy, m_aspectRatio, m_zNear, m_zFar);
    m_recalcProjectionMatrix = false;
  }
  return m_projectionMatrix;
}

void Projection::setFOV(float fovy) {
  // fovy angle is restricted between 0 and 180 degrees
  if (fovy < 0.0f)
    fovy = 0.0f;
  else if (fovy > 180.0f)
    fovy = 180.0f;

  m_fovy = fovy;
  m_recalcProjectionMatrix = true;
}

void Projection::setAspectRatio(float aspectRatio) {
  m_aspectRatio = aspectRatio;
  m_recalcProjectionMatrix = true;
}

void Projection::setZNear(float zNear) {
  m_zNear = zNear;
  m_recalcProjectionMatrix = true;
}

void Projection::setZFar(float zFar) {
  m_zFar = zFar;
  m_recalcProjectionMatrix = true;
}

void Projection::setProjectionMatrix(const glm::mat4 &projectionMatrix) {
  m_projectionMatrix = projectionMatrix;
}