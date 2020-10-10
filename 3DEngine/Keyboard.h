#pragma once

#include <GLFW/glfw3.h>

static constexpr int NUM_KEYS = 512;

class Keyboard {
public:
  static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void KeyControl();

private:
  Keyboard() {} // prevent construction of this class

  static bool _keys[NUM_KEYS];
};