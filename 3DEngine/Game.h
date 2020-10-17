#pragma once

#include "Camera.h"
#include "Window.h"

#include <memory>

#include <glm/glm.hpp>

// Camera statics
static constexpr float CAMERA_MOVE_SPEED = 5.0f;
static constexpr float CAMERA_TURN_SPEED = glm::radians(60.0f);
static constexpr float CAMERA_ZOOM_SPEED = glm::radians(25.0f);
static constexpr float CAMERA_MIN_ANGLE = glm::radians(-180.f);
static constexpr float CAMERA_MAX_ANGLE = glm::radians(180.f);
static constexpr float CAMERA_MIN_FOV = glm::radians(10.0f);
static constexpr float CAMERA_MAX_FOV = glm::radians(120.0f);
static constexpr float CAMERA_NEAR_PLANE = 0.1f;
static constexpr float CAMERA_FAR_PLANE = 100.0f;

// Camera setup params
static constexpr auto CAMERA_SETUP_POS = glm::vec3(0.0f, 0.0f, 0.0f);
static constexpr float CAMERA_SETUP_YAW = glm::radians(180.0f);
static constexpr float CAMERA_SETUP_PITCH = glm::radians(0.0f);
static constexpr float CAMERA_SETUP_FOV = glm::radians(45.0f);

// Window statics
static constexpr int WINDOW_WIDTH = 1600;
static constexpr int WINDOW_HEIGHT = 1200;
#ifdef _DEBUG
static constexpr bool USE_WINDOWED = true;
#else
static constexpr bool USE_WINDOWED = false;
#endif
static constexpr bool FULLSCREEN_WINDOWS = false;

// Game options
static constexpr float FPS_UPDATE_DELAY = 0.5f;
static constexpr size_t FPS_BUFFER_SIZE = 8;
static constexpr int MAX_LIGHTS = 8;
static constexpr bool USE_DEPTH_VISUALISATION = false;

// Text options
static constexpr auto HUD_FONT = "Unreal";
static constexpr auto COLOR_SEAWEED = glm::vec4(0.0392f, 0.4941f, 0.549f, 1.0f);
static constexpr auto COLOR_RED = glm::vec4(0.651f, 0.1725f, 0.1686f, 1.0f);
static constexpr auto COLOR_GREEN = glm::vec4(0.1608f, 0.4314f, 0.0039f, 1.0f);
static constexpr auto COLOR_BLUE = glm::vec4(0.1961f, 0.3216f, 0.4824f, 1.0f);
static constexpr auto COLOR_YELLOW = glm::vec4(0.9922f, 0.80f, 0.051f, 1.0f);
static constexpr auto COLOR_VIOLET = glm::vec4(0.3569f, 0.0392f, 0.5686f, 1.0f);
static constexpr auto COLOR_GREY = glm::vec4(0.6667f, 0.6627f, 0.6784f, 1.0f);

// Other
static constexpr float TWO_PI_RADIANS = glm::radians(360.0f);

class Game {
public:
  static void Init();
  static void Exit();

  static Camera &GetCamera();
  static Window &GetWindow();

private:
  Game() {} // prevent construction of this class

  static std::unique_ptr<Camera> _camera;
  static std::unique_ptr<Window> _window;
};