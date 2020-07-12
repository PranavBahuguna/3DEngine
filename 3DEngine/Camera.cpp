#include "Camera.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

#define MOVE_SPEED 5.0f
#define TURN_SPEED 60.0f

static CamPtr instance = nullptr;

// Constructor - zero all parameters
Camera::Camera()
    : m_position(0), m_up(0), m_yaw(0), m_pitch(0), m_right(0), m_front(0), m_worldUp(0), m_view(0),
      m_projection(0) {}

// Get camera singleton instance
CamPtr Camera::GetInstance() {
  if (instance == nullptr)
    instance = CamPtr(new Camera());

  return instance;
}

// Initialise all camera properties
void Camera::Init(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov,
                  float near, float far) {
  if (instance == nullptr)
    instance = GetInstance();

  instance->m_position = pos;
  instance->m_worldUp = up;
  instance->m_yaw = yaw;
  instance->m_pitch = pitch;

  instance->m_projection = glm::perspective(fov, Window::GetAspectRatio(), near, far);
  instance->updateDirection();
}

// Handles key input to the camera
void Camera::_keyControl() {
  float deltaTime = Timer::GetDeltaTime();
  const auto &keys = Window::GetKeys();

  if (keys[GLFW_KEY_W])
    m_position += m_front * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_S])
    m_position -= m_front * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_D])
    m_position += m_right * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_A])
    m_position -= m_right * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_SPACE])
    m_position += m_up * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_LEFT_CONTROL])
    m_position -= m_up * MOVE_SPEED * deltaTime;

  if (keys[GLFW_KEY_RIGHT])
    m_yaw += TURN_SPEED * deltaTime;

  if (keys[GLFW_KEY_LEFT])
    m_yaw -= TURN_SPEED * deltaTime;

  if (keys[GLFW_KEY_UP])
    m_pitch += TURN_SPEED * deltaTime;

  if (keys[GLFW_KEY_DOWN])
    m_pitch -= TURN_SPEED * deltaTime;
}

// Handles mouse input to the camera
void Camera::_mouseControl() {
  float deltaTime = Timer::GetDeltaTime();
  float deltaX = Window::GetDeltaX();
  float deltaY = Window::GetDeltaY();

  // Update yaw and pitch
  m_yaw += deltaX * TURN_SPEED * deltaTime;
  m_pitch += deltaY * TURN_SPEED * deltaTime;

  // Limit pitch to be under 90 degrees for now
  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  // Loop yaw value around if it exceeds +/- 180 degrees
  if (m_yaw > 180.0f)
    m_yaw -= 360.0f;
  if (m_yaw < -180.0f)
    m_yaw += 360.0f;

  updateDirection();
}

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

  // Calculate the camera view
  m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}