#pragma once

#include <glm/glm.hpp>

constexpr float CAMERA_MOVE_SPEED = 5.0f;
constexpr float CAMERA_TURN_SPEED = glm::radians(60.0f);
constexpr float CAMERA_ZOOM_SPEED = glm::radians(25.0f);
constexpr float CAMERA_MIN_ANGLE = glm::radians(-180.f);
constexpr float CAMERA_MAX_ANGLE = glm::radians(180.f);
constexpr float CAMERA_MIN_FOV = glm::radians(10.0f);
constexpr float CAMERA_MAX_FOV = glm::radians(120.0f);

constexpr float TWO_PI_RADIANS = glm::radians(360.0f);