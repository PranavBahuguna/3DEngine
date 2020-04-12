#include "Camera.h"

#include <GLFW\glfw3.h>

// Constructor
Camera::Camera(glm::vec3 postiion, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed,
               GLfloat turnSpeed)
    : m_position(postiion), m_worldUp(up), m_yaw(yaw), m_pitch(pitch), m_moveSpeed(moveSpeed),
      m_turnSpeed(turnSpeed) {

  update();
}

// Destructor
Camera::~Camera() {}

// Handles key input to the camera
void Camera::keyControl(bool *keys, GLfloat deltaTime) {

  GLfloat velocity = m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_W])
    m_position += m_front * velocity;

  if (keys[GLFW_KEY_S])
    m_position -= m_front * velocity;

  if (keys[GLFW_KEY_D])
    m_position += m_right * velocity;

  if (keys[GLFW_KEY_A])
    m_position -= m_right * velocity;

  if (keys[GLFW_KEY_SPACE])
    m_position += m_up * velocity;

  if (keys[GLFW_KEY_LEFT_CONTROL])
    m_position -= m_up * velocity;
}

// Update the camera's properties
void Camera::update() {
  // Calculate the front vector
  m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front.y = sin(glm::radians(m_pitch));
  m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(m_front);

  // Calculate the right and up vector
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}