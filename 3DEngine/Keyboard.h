#pragma once

#include "Camera.h"

#include <GLFW/glfw3.h>

static constexpr int NUM_KEYS = 512;

class Keyboard {
public:
  Keyboard(Keyboard const &) = delete;
  void operator=(Keyboard const &) = delete;
  static void Init();

  static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void KeyControl();

private:
  Keyboard(); // prevent construction of this class
  
  bool m_keys[NUM_KEYS];

  std::shared_ptr<Camera> m_mainCamera;
};