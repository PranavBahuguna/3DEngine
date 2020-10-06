#include "Mouse.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Timer.h"

#include <stdexcept>

static std::unique_ptr<Mouse> _instance = nullptr;

Mouse::Mouse()
    : m_lastX(0.0f), m_lastY(0.0f), m_deltaX(0.0f), m_deltaY(0.0f), m_offsetX(0.0f),
      m_offsetY(0.0f), m_mouseFirstMoved(false) {}

void Mouse::Init() {
  if (_instance == nullptr)
    _instance = std::unique_ptr<Mouse>(new Mouse());

  _instance->m_lastX = 0.0f;
  _instance->m_lastY = 0.0f;
  _instance->m_deltaX = 0.0f;
  _instance->m_deltaY = 0.0f;
  _instance->m_offsetX = 0.0f;
  _instance->m_offsetY = 0.0f;
  _instance->m_mouseFirstMoved = true;

  if (ResourceManager<Camera>::FindOrError("main", _instance->m_mainCamera) != ERROR_OK)
    throw std::runtime_error("An error occurred while initializing Mouse.");
}

// Handles mouse movement input
void Mouse::MouseHandler(GLFWwindow *window, double xPos, double yPos) {
  float fxPos = static_cast<float>(xPos);
  float fyPos = static_cast<float>(yPos);

  if (_instance->m_mouseFirstMoved) {
    _instance->m_lastX = fxPos;
    _instance->m_lastY = fyPos;
    _instance->m_mouseFirstMoved = false;
  }

  _instance->m_deltaX = fxPos - _instance->m_lastX;
  _instance->m_deltaY = _instance->m_lastY - fyPos;

  _instance->m_lastX = fxPos;
  _instance->m_lastY = fyPos;
}

// Handles mouse scroll input
void Mouse::MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset) {
  _instance->m_offsetX = static_cast<float>(xOffset);
  _instance->m_offsetY = static_cast<float>(yOffset);
}

void Mouse::MouseControl() {
  float deltaYaw = GetDeltaX() * CAMERA_TURN_SPEED * Timer::GetDeltaTime();
  float deltaPitch = GetDeltaY() * CAMERA_TURN_SPEED * Timer::GetDeltaTime();

  // Update yaw and pitch
  _instance->m_mainCamera->performAction(CameraAction::TurnRight, deltaYaw);
  _instance->m_mainCamera->performAction(CameraAction::TurnUp, deltaPitch);
}

void Mouse::MouseScrollControl() {
  float yScrollAmount = GetOffsetY() * CAMERA_ZOOM_SPEED * Timer::GetDeltaTime();

  // Update fov
  _instance->m_mainCamera->performAction(CameraAction::Zoom, yScrollAmount);
}

// Obtains and resets change in x-position
float Mouse::GetDeltaX() {
  float deltaX = _instance->m_deltaX;
  _instance->m_deltaX = 0.0f;
  return deltaX;
}

// Obtains and resets change in y-position
float Mouse::GetDeltaY() {
  float deltaY = _instance->m_deltaY;
  _instance->m_deltaY = 0.0f;
  return deltaY;
}

float Mouse::GetOffsetX() { return _instance->m_offsetX; }

float Mouse::GetOffsetY() { return _instance->m_offsetY; }