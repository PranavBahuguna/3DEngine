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

  float getFOV() const;
  float getAspectRatio() const;
  float getNearPlane() const;
  float getFarPlane() const;
  const glm::mat4 &getProjection() const;

private:
  void updateProjection() const;

  float m_fovy;
  float m_aspectRatio;
  float m_zNear;
  float m_zFar;

  mutable glm::mat4 m_projection;
  mutable bool m_projectionUpdated;
};