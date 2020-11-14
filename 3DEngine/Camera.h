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

  Transform &transform();
  Projection &projection();

  void performAction(CameraAction action, float amount);

private:
  Transform m_transform;
  Projection m_projection;
};
