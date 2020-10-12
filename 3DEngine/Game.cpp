#include "Game.h"

std::unique_ptr<Camera> Game::_camera = nullptr;
std::unique_ptr<Window> Game::_window = nullptr;

// Initialise all basic components required to run the game
void Game::Init() {
  // Determine the window mode to use and create a window
  WindowMode wMode = (!USE_WINDOWED)        ? WindowMode::FULLSCREEN
                     : (FULLSCREEN_WINDOWS) ? WindowMode::FULLSCREEN_WINDOWED
                                            : WindowMode::WINDOWED;
  _window = std::unique_ptr<Window>(new Window("Test window", wMode, WINDOW_WIDTH, WINDOW_HEIGHT));

  // Setup camera
  _camera = std::unique_ptr<Camera>(
      new Camera(CAMERA_SETUP_POS, glm::vec3(0, 1, 0), CAMERA_SETUP_YAW, CAMERA_SETUP_PITCH,
                 CAMERA_SETUP_FOV, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE));

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

Camera &Game::GetCamera() { return *_camera; }

Window &Game::GetWindow() { return *_window; }