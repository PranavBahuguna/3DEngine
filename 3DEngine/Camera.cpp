#include "Camera.h"

#include <GLFW\glfw3.h>

// Constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed,
               GLfloat turnSpeed)
    : m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch), m_moveSpeed(moveSpeed),
      m_turnSpeed(turnSpeed) {

  updateDirection();
}

// Destructor
Camera::~Camera() {}

// Handles key input to the camera
void Camera::keyControl(const bool* keys, GLfloat deltaTime) {

  if (keys[GLFW_KEY_W])
    m_position += m_front * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_S])
    m_position -= m_front * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_D])
    m_position += m_right * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_A])
    m_position -= m_right * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_SPACE])
    m_position += m_up * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_LEFT_CONTROL])
    m_position -= m_up * m_moveSpeed * deltaTime;

  if (keys[GLFW_KEY_RIGHT])
    m_yaw += m_turnSpeed * deltaTime;

  if (keys[GLFW_KEY_LEFT])
    m_yaw -= m_turnSpeed * deltaTime;

  if (keys[GLFW_KEY_UP])
    m_pitch += m_turnSpeed * deltaTime;

  if (keys[GLFW_KEY_DOWN])
    m_pitch -= m_turnSpeed * deltaTime;
}

// Handles mouse input to the camera
void Camera::mouseControl(GLfloat deltaX, GLfloat deltaY, GLfloat deltaTime) {
  // Update yaw and pitch
  m_yaw += deltaX * m_turnSpeed * deltaTime;
  m_pitch += deltaY * m_turnSpeed * deltaTime;

  // Limit pitch to be under 90 degrees for now
  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  updateDirection();
}

// Set the camera projection
void Camera::setProjection(float fov, float aspectRatio, float near, float far) {
  m_projection = glm::perspective(fov, aspectRatio, near, far);
}

// Get the camera view
glm::mat4 Camera::getView() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

// Update the camera's direction
void Camera::updateDirection() {
  // Calculate the front vector
  m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front.y = sin(glm::radians(m_pitch));
  m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(m_front);

  // Calculate the right and up vector
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}