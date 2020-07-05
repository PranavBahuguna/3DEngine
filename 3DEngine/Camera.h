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
  static CamPtr getInstance();

  void keyControl(const bool *keys, GLfloat deltaTime);
  void mouseControl(GLfloat deltaX, GLfloat deltaY, GLfloat deltaTime);

  glm::mat4 getView() const { return m_view; }
  glm::mat4 getProjection() const { return m_projection; }
  glm::vec3 getPosition() const { return m_position; }
  GLfloat getPitch() const { return m_pitch; }
  GLfloat getYaw() const { return m_yaw; }

private:
  Camera(); // prevent construction of this class

  void updateDirection();

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
