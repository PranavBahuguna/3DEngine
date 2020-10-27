#include "Game.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"

#include <iomanip>
#include <sstream>

std::unique_ptr<Camera> Game::_camera = nullptr;
std::unique_ptr<UiOverlay> Game::_uiOverlay = nullptr;
std::unique_ptr<Window> Game::_window = nullptr;
bool Game::_updateScene = true;

// Initialise all basic components required to run the game
void Game::Init() {
  // Determine the window mode to use and create a window
  WindowMode wMode = (!USE_WINDOWED)        ? WindowMode::FULLSCREEN
                     : (FULLSCREEN_WINDOWS) ? WindowMode::FULLSCREEN_WINDOWED
                                            : WindowMode::WINDOWED;
  _window = std::unique_ptr<Window>(new Window("Test window", wMode, WINDOW_WIDTH, WINDOW_HEIGHT));

  // Setup camera
  View cameraView(Transform(CAMERA_SETUP_POS, CAMERA_SETUP_ROTATION));
  _camera = std::unique_ptr<Camera>(
      new Camera(cameraView, CAMERA_SETUP_FOV, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE));

  // Setup UI
  _uiOverlay = std::unique_ptr<UiOverlay>(new UiOverlay());

  // Allow objects to obscure other objects behind them
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Handles closing of game session and window
void Game::Exit() { _window->close(); }

// Toggle the scene update status
void Game::ToggleSceneUpdateStatus() { _updateScene = !_updateScene; }

void Game::Update() {
  Timer::Update();

  // Handle user input events
  glfwPollEvents();
  Keyboard::KeyControl();
  Mouse::MouseControl();
  Mouse::MouseScrollControl();

  _camera->update();

  // Clear window
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Camera &Game::GetCamera() { return *_camera; }

Window &Game::GetWindow() { return *_window; }

UiOverlay &Game::GetUiOverlay() { return *_uiOverlay; }

bool Game::ShouldUpdateScene() { return _updateScene; }

// Converts a float to string with a number of decimal places
std::string Game::toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}