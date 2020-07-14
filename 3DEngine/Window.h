#pragma once

#include "Error.h"

#include <memory>
#include <string>

// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// clang-format on

#define NUM_KEYS 512

class Window;
typedef std::unique_ptr<Window> WndPtr;

enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

class Window {
public:
  ~Window();

  static void Init(const std::string &name, WindowMode wMode, ERROR &errCode);
  static void Init(const std::string &name, WindowMode wMode, int width, int height,
                   ERROR &errCode);

  static int GetWidth();
  static int GetHeight();
  static float GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); }
  static glm::vec2 RelToWinPos(const glm::vec2 &pos) {
    return {pos.x * GetWidth(), pos.y * GetHeight()};
  }
  static float GetDeltaX();
  static float GetDeltaY();
  static float GetOffsetX();
  static float GetOffsetY();

  static bool GetShouldClose();
  static void SwapBuffers();

  static const bool *GetKeys();
  static bool GetToggleKey(int key, ERROR *errCode);

private:
  Window(); // prevent construction of this class

  static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void MouseHandler(GLFWwindow *window, double xPos, double yPos);
  static void MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset);

  GLFWwindow *m_mainWindow;

  int m_width;
  int m_height;

  bool m_keys[NUM_KEYS];
  bool m_toggleKeys[NUM_KEYS];

  float m_lastX;
  float m_lastY;
  float m_deltaX;
  float m_deltaY;
  float m_offsetX;
  float m_offsetY;
  bool m_mouseFirstMoved;
};
