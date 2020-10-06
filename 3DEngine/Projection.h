#pragma once

#include <glm/glm.hpp>

enum class ProjectionType {
  perspective,
  orthographic
};

class Projection {
public:
  Projection();
  Projection(float left, float right, float bottom, float top, float zNear, float zFar);
  Projection(float fovy, float aspectRatio, float zNear, float zFar);

  virtual ~Projection();

  float getFOV() const { return m_fovy; }
  float getAspectRatio() const { return m_aspectRatio; }
  float getZNear() const { return m_zNear; }
  float getZFar() const { return m_zFar; }
  ProjectionType getProjectionType() const { return m_projectionType; }
  glm::mat4 getProjectionMatrix() const;

  void setFOV(float fovy);
  void setAspectRatio(float aspectRatio);
  void setZNear(float zNear);
  void setZFar(float zFar);
  void setProjectionMatrix(const glm::mat4 &projectionMatrix);

protected:
  float m_fovy;
  float m_aspectRatio;
  float m_zNear;
  float m_zFar;
  ProjectionType m_projectionType;
  
  mutable bool m_recalcProjectionMatrix;
  mutable glm::mat4 m_projectionMatrix;
};