#pragma once

#include <glm/glm.hpp>

class Projection {
public:
  Projection(float fovy, float aspectRatio, float zNear, float zFar);

  // All mutator methods return the current Projection to allow for method chaining
  Projection &setFOV(float fovy);
  Projection &zoom(float zoomAmount);
  Projection &setAspectRatio(float aspectRatio);
  Projection &setNearPlane(float zNear);
  Projection &setFarPlane(float zFar);

  float getFOV();
  float getAspectRatio();
  float getNearPlane();
  float getFarPlane();
  const glm::mat4 &getProjection();

private:
  void updateProjectionMatrix();

  float m_fovy;
  float m_aspectRatio;
  float m_zNear;
  float m_zFar;
  glm::mat4 m_projection;

  bool m_projectionMatrixUpdated;
};