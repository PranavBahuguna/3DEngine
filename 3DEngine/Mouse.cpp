#include "Mouse.h"
#include "Game.h"
#include "Timer.h"

float Mouse::_lastX = 0.0f;
float Mouse::_lastY = 0.0f;
float Mouse::_deltaX = 0.0f;
float Mouse::_deltaY = 0.0f;
float Mouse::_offsetX = 0.0f;
float Mouse::_offsetY = 0.0f;
bool Mouse::_mouseFirstMoved = true;

// Handles mouse movement input
void Mouse::MouseHandler(GLFWwindow *window, double xPos, double yPos) {
  float fxPos = static_cast<float>(xPos);
  float fyPos = static_cast<float>(yPos);

  if (_mouseFirstMoved) {
    _lastX = fxPos;
    _lastY = fyPos;
    _mouseFirstMoved = false;
  }

  _deltaX = fxPos - _lastX;
  _deltaY = _lastY - fyPos;

  _lastX = fxPos;
  _lastY = fyPos;
}

// Handles mouse scroll input
void Mouse::MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset) {
  _offsetX = static_cast<float>(xOffset);
  _offsetY = static_cast<float>(yOffset);
}

void Mouse::MouseControl() {
  float deltaYaw = _deltaX * CAMERA_TURN_SPEED * Timer::GetDeltaTime();
  float deltaPitch = _deltaY * CAMERA_TURN_SPEED * Timer::GetDeltaTime();

  // Update yaw and pitch
  Game::GetCamera().performAction(CameraAction::TurnRight, deltaYaw);
  Game::GetCamera().performAction(CameraAction::TurnUp, deltaPitch);

  // Reset changes in x/y-position
  _deltaX = 0.0f;
  _deltaY = 0.0f;
}

void Mouse::MouseScrollControl() {
  float yScrollAmount = _offsetY * CAMERA_ZOOM_SPEED * Timer::GetDeltaTime();

  // Update fov
  Game::GetCamera().performAction(CameraAction::Zoom, yScrollAmount);
}