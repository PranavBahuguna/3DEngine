#pragma once

#include "Camera.h"
#include "Window.h"

#include <memory>

#include <glm/glm.hpp>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200
#ifdef _DEBUG
#define USE_WINDOWED true
#else
#define USE_WINDOWED false
#endif
#define FULLSCREEN_WINDOWS false

static constexpr float CAMERA_MOVE_SPEED = 5.0f;
static constexpr float CAMERA_TURN_SPEED = glm::radians(60.0f);
static constexpr float CAMERA_ZOOM_SPEED = glm::radians(25.0f);
static constexpr float CAMERA_MIN_ANGLE = glm::radians(-180.f);
static constexpr float CAMERA_MAX_ANGLE = glm::radians(180.f);
static constexpr float CAMERA_MIN_FOV = glm::radians(10.0f);
static constexpr float CAMERA_MAX_FOV = glm::radians(120.0f);
static constexpr float CAMERA_NEAR_PLANE = 0.1f;
static constexpr float CAMERA_FAR_PLANE = 100.0f;

static constexpr float TWO_PI_RADIANS = glm::radians(360.0f);

class Game {
public:
  static void Init();
  static void Exit();

  static Camera &GetCamera();
  static Window &GetWindow();

private:
  Game() {} // prevent construction of this class

  static std::unique_ptr<Camera> CameraPtr;
  static std::unique_ptr<Window> WindowPtr;
};