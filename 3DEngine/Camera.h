#pragma once

#include "Error.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

enum class CameraAction { MoveFront, MoveRight, MoveUp, TurnRight, TurnUp, Zoom };

class Camera {
public:
  Camera(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov, float near,
         float far);
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
  void updateDirection();
  void updateProjection();
  void restrictAngle(float &angle) const;

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
