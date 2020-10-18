// clang-format off
#include "Window.h"
#include "Keyboard.h"
#include "Camera.h"
#include "Game.h"
#include "Timer.h"
// clang-format on

KeyState Keyboard::_keys[NUM_KEYS] = {};

void Keyboard::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  _keys[key].update(action);
}

// This should be called in the main game loop to handle key input state for every frame
void Keyboard::KeyControl() {
  float deltaTime = Timer::GetDeltaTime();
  auto &camera = Game::GetCamera();

  if (_keys[GLFW_KEY_W].isPressed())
    camera.performAction(CameraAction::MoveFront, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_S].isPressed())
    camera.performAction(CameraAction::MoveFront, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_D].isPressed())
    camera.performAction(CameraAction::MoveRight, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_A].isPressed())
    camera.performAction(CameraAction::MoveRight, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_SPACE].isPressed())
    camera.performAction(CameraAction::MoveUp, CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_LEFT_CONTROL].isPressed())
    camera.performAction(CameraAction::MoveUp, -CAMERA_MOVE_SPEED * deltaTime);

  if (_keys[GLFW_KEY_RIGHT].isPressed())
    camera.performAction(CameraAction::TurnRight, CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_LEFT].isPressed())
    camera.performAction(CameraAction::TurnRight, -CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_UP].isPressed())
    camera.performAction(CameraAction::TurnUp, CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_DOWN].isPressed())
    camera.performAction(CameraAction::TurnUp, -CAMERA_TURN_SPEED * deltaTime);

  if (_keys[GLFW_KEY_O].isPressed())
    camera.performAction(CameraAction::Zoom, CAMERA_ZOOM_SPEED * deltaTime);

  if (_keys[GLFW_KEY_P].isPressed())
    camera.performAction(CameraAction::Zoom, -CAMERA_ZOOM_SPEED * deltaTime);

  if (_keys[GLFW_KEY_M].isToggled())
    Game::GetUiOverlay().toggle();

  if (_keys[GLFW_KEY_ENTER].isToggled())
    Game::ToggleSceneUpdateStatus();

  if (_keys[GLFW_KEY_ESCAPE].isToggled())
    Game::Exit();
}