#include "Camera.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

#define MOVE_SPEED 5.0f
#define TURN_SPEED 60.0f
#define ZOOM_SPEED 25.0f
#define MIN_FOV 10.0f
#define MAX_FOV 120.0f

static CamPtr _instance = nullptr;

// Constructor - zero all parameters
Camera::Camera()
    : m_position(0), m_up(0), m_yaw(0), m_pitch(0), m_right(0), m_front(0), m_worldUp(0), m_fov(0),
      m_near(0), m_far(0), m_view(0), m_projection(0) {}

// Initialise all camera properties
void Camera::Init(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov,
                  float near, float far) {
  if (_instance == nullptr)
    _instance = CamPtr(new Camera());

  _instance->m_position = pos;
  _instance->m_worldUp = up;
  _instance->m_yaw = yaw;
  _instance->m_pitch = pitch;

  _instance->m_fov = fov;
  _instance->m_near = near;
  _instance->m_far = far;

  _instance->updateDirection();
  _instance->updateProjection();
}

// Static handler utility methods
glm::mat4 Camera::GetView() { return _instance->m_view; }

glm::mat4 Camera::GetProjection() { return _instance->m_projection; }

glm::vec3 Camera::GetPosition() { return _instance->m_position; }

float Camera::GetPitch() { return _instance->m_pitch; }

float Camera::GetYaw() { return _instance->m_yaw; }

float Camera::GetFOV() { return _instance->m_fov; }

float Camera::GetNear() { return _instance->m_near; }

float Camera::GetFar() { return _instance->m_far; }

void Camera::KeyControl() { _instance->_keyControl(); };

void Camera::MouseControl() { _instance->_mouseControl(); };

void Camera::MouseScrollControl() { _instance->_mouseScrollControl(); }

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

  if (keys[GLFW_KEY_O]) {
    m_fov -= ZOOM_SPEED * deltaTime;
    _instance->updateProjection();
  }

  if (keys[GLFW_KEY_P]) {
    m_fov += ZOOM_SPEED * deltaTime;
    _instance->updateProjection();
  }
}

// Handles mouse movement input to the camera
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

// Handles mouse scroll input to the camera
void Camera::_mouseScrollControl() {
  float deltaTime = Timer::GetDeltaTime();
  float yOffset = Window::GetOffsetY();

  // Update fov
  _instance->m_fov += yOffset * ZOOM_SPEED * deltaTime;

  // Clamp fov values
  if (_instance->m_fov < MIN_FOV)
    _instance->m_fov = MIN_FOV;
  if (_instance->m_fov > MAX_FOV)
    _instance->m_fov = MAX_FOV;

  _instance->updateProjection();
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

// Update the camera's projection
void Camera::updateProjection() {
  m_projection =
      glm::perspective(glm::radians(_instance->m_fov), Window::GetAspectRatio(), m_near, m_far);
}