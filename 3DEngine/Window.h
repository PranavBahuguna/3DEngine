#pragma once

// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// clang-format on

#include "Error.h"

#include <memory>
#include <string>

class Window;
typedef std::unique_ptr<Window> WndPtr;

enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

class Window {
public:
  ~Window();

  static void Init(const std::string &name, WindowMode wMode);
  static void Init(const std::string &name, WindowMode wMode, int width, int height);

  static int GetWidth();
  static int GetHeight();
  static float GetAspectRatio();
  static glm::vec2 RelToWinPos(const glm::vec2 &pos);

  static bool GetShouldClose();
  static void SwapBuffers();

private:
  Window(); // prevent construction of this class

  GLFWwindow *m_mainWindow;

  int m_width;
  int m_height;
};
