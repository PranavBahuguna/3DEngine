#pragma once

#include <GLFW/glfw3.h>

static constexpr int NUM_KEYS = 512;

struct KeyState {
public:
  void update(int action) {
    oldState = newState;
    newState = action;
  }

  bool isPressed(bool includeRepeat = true) {
    return newState == GLFW_PRESS || (includeRepeat && newState == GLFW_REPEAT);
  }

  // A key can only be toggled if pressed when it was previously released. This ensures it only
  // returns true once when pressed and held.
  bool isToggled() {
    if (newState == GLFW_PRESS && oldState == GLFW_RELEASE) {
      update(GLFW_RELEASE); // prevents any held presses from returning true
      return true;
    }

    return false;
  }

private:
  int oldState;
  int newState;
};

class Keyboard {
public:
  static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void KeyControl();

private:
  Keyboard() {} // prevent construction of this class

  static KeyState _keys[NUM_KEYS];
};