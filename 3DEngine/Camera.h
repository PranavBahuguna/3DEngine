#pragma once

#include "Error.h"
#include "ResourceManager.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

enum class CameraAction {
  MoveFront,
  MoveRight,
  MoveUp,
  TurnRight,
  TurnUp,
  Zoom
};

class Camera : public Resource {
public:
  Camera(const std::string &name, const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch,
         float fov, float near, float far);

  void update();

  glm::mat4 getView();
  glm::mat4 getProjection();
  glm::vec3 getPosition();
  float getPitch();
  float getYaw();
  float getFOV();
  float getNearPlane();
  float getFarPlane();

  void performAction(CameraAction action, float amount);

private:
  void updateDirection();
  void updateProjection();
  void restrictAngle(float &angle);

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;

  float m_yaw;
  float m_pitch;
  float m_fovy;
  float m_zNear;
  float m_zFar;

  glm::quat m_orientation;
  glm::mat4 m_view;
  glm::mat4 m_projection;

  bool m_recalcView;
  bool m_recalcProjection;
};
