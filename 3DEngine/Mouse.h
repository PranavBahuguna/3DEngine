#pragma once

#include "Camera.h"

#include <GLFW/glfw3.h>

class Mouse {
public:
  Mouse(Mouse const &) = delete;
  void operator=(Mouse const &) = delete;
  static void Init();

  static void MouseHandler(GLFWwindow *window, double xPos, double yPos);
  static void MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset);

  static void MouseControl();
  static void MouseScrollControl();

private:
  Mouse(); // prevent construction of this class

  static float GetDeltaX();
  static float GetDeltaY();
  static float GetOffsetX();
  static float GetOffsetY();

  float m_lastX;
  float m_lastY;
  float m_deltaX;
  float m_deltaY;
  float m_offsetX;
  float m_offsetY;
  bool m_mouseFirstMoved;

  std::shared_ptr<Camera> m_mainCamera;
};