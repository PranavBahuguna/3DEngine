#pragma once

#include <GLFW/glfw3.h>

class Timer {
public:
  static void Update();
  static float GetDeltaTime();

private:
  Timer() {} // prevent construction of this class
};