#pragma once

#include "Error.h"
#include "Window.h"

#include <memory>

#include <glm/glm.hpp>

class Camera;
typedef std::shared_ptr<Camera> CamPtr;

class Camera {
public:
  static void Init(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov,
                   float near, float far);

  static void KeyControl() { GetInstance()->_keyControl(); };
  static void MouseControl() { GetInstance()->_mouseControl(); };

  static glm::mat4 GetView() { return GetInstance()->m_view; }
  static glm::mat4 GetProjection() { return GetInstance()->m_projection; }
  static glm::vec3 GetPosition() { return GetInstance()->m_position; }
  static float GetPitch() { return GetInstance()->m_pitch; }
  static float GetYaw() { return GetInstance()->m_yaw; }

private:
  Camera(); // prevent construction of this class

  static CamPtr GetInstance();

  void updateDirection();
  void _keyControl();
  void _mouseControl();

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;

  glm::mat4 m_view;
  glm::mat4 m_projection;
};
