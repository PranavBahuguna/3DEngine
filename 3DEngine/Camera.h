#pragma once

#include "Error.h"
#include "Window.h"

#include <memory>

#include <glm/glm.hpp>

class Camera;
typedef std::shared_ptr<Camera> CamPtr;

class Camera {
public:
  // Prevent copying of this camera object
  Camera(Camera const &) = delete;
  void operator=(Camera const &) = delete;

  static void init(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat fov,
                   GLfloat aspectRatio, GLfloat near, GLfloat far);

  static void keyControl(const bool *keys);
  static void mouseControl(float deltaX, float deltaY);

  static glm::mat4 getView() { return getInstance()->m_view; }
  static glm::mat4 getProjection() { return getInstance()->m_projection; }
  static glm::vec3 getPosition() { return getInstance()->m_position; }
  static GLfloat getPitch() { return getInstance()->m_pitch; }
  static GLfloat getYaw()  { return getInstance()->m_yaw; }

private:
  Camera(); // prevent construction of this class

  static CamPtr getInstance();

  void updateDirection();
  void _keyControl(const bool *keys);
  void _mouseControl(GLfloat deltaX, GLfloat deltaY);

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  GLfloat m_yaw;
  GLfloat m_pitch;

  glm::mat4 m_view;
  glm::mat4 m_projection;
};
