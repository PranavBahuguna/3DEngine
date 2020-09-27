#include "Camera.h"
#include "Timer.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float MOVE_SPEED = 5.0f;
constexpr float TURN_SPEED = glm::radians(60.0f);
constexpr float ZOOM_SPEED = glm::radians(25.0f);
constexpr float MIN_FOV = glm::radians(10.0f);
constexpr float MAX_FOV = glm::radians(120.0f);

static CamPtr _instance = nullptr;

// Constructor - zero all parameters
Camera::Camera()
    : m_position(0), m_up(0), m_yaw(0), m_pitch(0), m_right(0), m_front(0), m_worldUp(0), m_fov(0),
      m_near(0), m_far(0), m_orientation(0, 0, 0, 1), m_view(0), m_projection(0) {}

// Initialise all camera properties
void Camera::Init(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch, float fov,
                  float near, float far) {
  if (_instance == nullptr)
    _instance = CamPtr(new Camera());

  _instance->m_position = pos;
  _instance->m_worldUp = up;
  _instance->m_yaw = glm::radians(yaw);
  _instance->m_pitch = glm::radians(pitch);

  _instance->m_fov = glm::radians(fov);
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

void Camera::Update() {
  if (_instance->m_recalcView) {
    _instance->updateDirection();
    _instance->m_recalcView = false;
  }
  if (_instance->m_recalcProjection) {
    _instance->updateProjection();
    _instance->m_recalcProjection = false;
  }
}

// Handles key input to the camera
void Camera::_keyControl() {
  float deltaTime = Timer::GetDeltaTime();
  const auto &keys = Window::GetKeys();

  if (keys[GLFW_KEY_W]) {
    m_position += m_front * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_S]) {
    m_position -= m_front * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_D]) {
    m_position += m_right * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_A]) {
    m_position -= m_right * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_SPACE]) {
    m_position += m_up * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_LEFT_CONTROL]) {
    m_position -= m_up * MOVE_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_RIGHT]) {
    m_yaw += TURN_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_LEFT]) {
    m_yaw -= TURN_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_UP]) {
    m_pitch += TURN_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_DOWN]) {
    m_pitch -= TURN_SPEED * deltaTime;
    m_recalcView = true;
  }

  if (keys[GLFW_KEY_O]) {
    m_fov -= ZOOM_SPEED * deltaTime;
    m_recalcProjection = true;
  }

  if (keys[GLFW_KEY_P]) {
    m_fov += ZOOM_SPEED * deltaTime;
    m_recalcProjection = true;
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
}

// Update the camera's direction
void Camera::updateDirection() {
  restrictAngle(m_yaw);
  restrictAngle(m_pitch);

  // Calculate orientation from pitch and yaw
  glm::quat qPitch = glm::angleAxis(-m_pitch, glm::vec3(1, 0, 0));
  glm::quat qYaw = glm::angleAxis(m_yaw, glm::vec3(0, 1, 0));
  m_orientation = qPitch * qYaw;

  // Calculate view matrix
  m_view = glm::mat4_cast(m_orientation);       // rotation
  m_view = glm::translate(m_view, -m_position); // translation

  // Calculate the front, right and up vectors
  glm::quat invOrientation = glm::conjugate(m_orientation);
  m_front = glm::rotate(invOrientation, glm::vec3(0, 0, -1));
  m_right = glm::rotate(invOrientation, glm::vec3(1, 0, 0));
  m_up = glm::rotate(invOrientation, glm::vec3(0, 1, 0));
}

// Update the camera's projection
void Camera::updateProjection() {
  m_projection =
      glm::perspective(_instance->m_fov, Window::GetAspectRatio(), m_near, m_far);
}

// Restricts an angle between +/- 180 degrees
void Camera::restrictAngle(float &angle) {
  if (angle > glm::pi<float>())
    angle -= glm::two_pi<float>();
  if (angle < -glm::pi<float>())
    angle += glm::two_pi<float>();
}