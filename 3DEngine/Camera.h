#pragma once

#include "Error.h"
#include "Projection.h"
#include "Transform.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

enum class CameraAction { MoveFront, MoveRight, MoveUp, TurnRight, TurnUp, Zoom };

class Camera {
public:
  Camera(const Transform &transform, const Projection &projection);
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

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
  Transform m_transform;
  Projection m_projection;
};
