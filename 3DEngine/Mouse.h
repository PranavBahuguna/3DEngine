#pragma once

#include "Camera.h"

#include <GLFW/glfw3.h>

class Mouse {
public:
  static void MouseHandler(GLFWwindow *window, double xPos, double yPos);
  static void MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset);
  static void MouseControl();
  static void MouseScrollControl();

private:
  Mouse() {} // prevent construction of this class

  static float _lastX;
  static float _lastY;
  static float _deltaX;
  static float _deltaY;
  static float _offsetX;
  static float _offsetY;
  static bool _mouseFirstMoved;
};