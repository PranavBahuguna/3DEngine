#pragma once

#include "Error.h"
#include "View.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

enum class CameraAction { MoveFront, MoveRight, MoveUp, TurnRight, TurnUp, Zoom };

class Camera {
public:
  Camera(const View &view, float fov, float near, float far);
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

  void update();

  glm::mat4 getView() const;
  glm::mat4 getProjection() const;
  glm::vec3 getPosition() const;
  float getPitch() const;
  float getYaw() const;
  float getFOV() const;
  float getZNear() const;
  float getZFar() const;

  void performAction(CameraAction action, float amount);

private:
  void updateProjection();

  glm::mat4 m_projection;
  mutable View m_view;

  float m_fovy;
  float m_zNear;
  float m_zFar;

  bool m_recalcProjection;
};
