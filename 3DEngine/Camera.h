#pragma once

#include "Error.h"
#include "Window.h"

#include <memory>

#include <glm/glm.hpp>

class Camera;
typedef std::unique_ptr<Camera> CamPtr;

class Camera {
public:
  ~Camera() {}

  static void Init(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov,
                   float near, float far);

  static void KeyControl();
  static void MouseControl();
  static void MouseScrollControl();

  static glm::mat4 GetView();
  static glm::mat4 GetProjection();
  static glm::vec3 GetPosition();
  static float GetPitch();
  static float GetYaw();
  static float GetFOV();
  static float GetNear();
  static float GetFar();

  Camera(); // prevent construction of this class

private:
  void updateDirection();
  void updateProjection();

  void _keyControl();
  void _mouseControl();
  void _mouseScrollControl();

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;

  float m_fov;
  float m_near;
  float m_far;

  glm::mat4 m_view;
  glm::mat4 m_projection;
};
