// clang-format off
#include "Window.h"
#include "Keyboard.h"
#include "Camera.h"
#include "Game.h"
#include "Timer.h"
// clang-format on

bool Keyboard::_keys[NUM_KEYS] = {false};

void Keyboard::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  if (action == GLFW_PRESS)
    _keys[key] = true;
  if (action == GLFW_RELEASE)
    _keys[key] = false;
}

void Keyboard::KeyControl() {
  float deltaTime = Timer::GetDeltaTime();
  auto &camera = Game::GetCamera();

  if (_keys[GLFW_KEY_W])
    camera.performAction(CameraAction::MoveFront, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_S])
    camera.performAction(CameraAction::MoveFront, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_D])
    camera.performAction(CameraAction::MoveRight, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_A])
    camera.performAction(CameraAction::MoveRight, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_SPACE])
    camera.performAction(CameraAction::MoveUp, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_LEFT_CONTROL])
    camera.performAction(CameraAction::MoveUp, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_RIGHT])
    camera.performAction(CameraAction::TurnRight, CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_LEFT])
    camera.performAction(CameraAction::TurnRight, -CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_UP])
    camera.performAction(CameraAction::TurnUp, CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_DOWN])
    camera.performAction(CameraAction::TurnUp, -CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_O])
    camera.performAction(CameraAction::Zoom, CAMERA_ZOOM_SPEED * deltaTime);

  if (_keys[GLFW_KEY_P])
    camera.performAction(CameraAction::Zoom, -CAMERA_ZOOM_SPEED * deltaTime);

  if (_keys[GLFW_KEY_ESCAPE])
    Game::Exit();
}