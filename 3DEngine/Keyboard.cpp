#include "Keyboard.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "Window.h"

#include <algorithm>
#include <stdexcept>

static std::unique_ptr<Keyboard> _instance = nullptr;

Keyboard::Keyboard() : m_keys{} {}

void Keyboard::Init() {
  if (_instance == nullptr)
    _instance = std::unique_ptr<Keyboard>(new Keyboard());

  std::fill_n(_instance->m_keys, NUM_KEYS, false);
  if (ResourceManager<Camera>::FindOrError("main", _instance->m_mainCamera) != ERROR_OK)
    throw std::runtime_error("An error occurred while initializing Keyboard.");
}

void Keyboard::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (action == GLFW_PRESS)
    _instance->m_keys[key] = true;
  if (action == GLFW_RELEASE)
    _instance->m_keys[key] = false;
}

void Keyboard::KeyControl() {
  float deltaTime = Timer::GetDeltaTime();

  if (_instance->m_keys[GLFW_KEY_W])
    _instance->m_mainCamera->performAction(CameraAction::MoveFront, CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_S])
    _instance->m_mainCamera->performAction(CameraAction::MoveFront, -CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_D])
    _instance->m_mainCamera->performAction(CameraAction::MoveRight, CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_A])
    _instance->m_mainCamera->performAction(CameraAction::MoveRight, -CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_SPACE])
    _instance->m_mainCamera->performAction(CameraAction::MoveUp, CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_LEFT_CONTROL])
    _instance->m_mainCamera->performAction(CameraAction::MoveUp, -CAMERA_MOVE_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_RIGHT])
    _instance->m_mainCamera->performAction(CameraAction::TurnRight, CAMERA_TURN_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_LEFT])
    _instance->m_mainCamera->performAction(CameraAction::TurnRight, -CAMERA_TURN_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_UP])
    _instance->m_mainCamera->performAction(CameraAction::TurnUp, CAMERA_TURN_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_DOWN])
    _instance->m_mainCamera->performAction(CameraAction::TurnUp, -CAMERA_TURN_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_O])
    _instance->m_mainCamera->performAction(CameraAction::Zoom, CAMERA_ZOOM_SPEED * deltaTime);

  if (_instance->m_keys[GLFW_KEY_P])
    _instance->m_mainCamera->performAction(CameraAction::Zoom, -CAMERA_ZOOM_SPEED * deltaTime);
}