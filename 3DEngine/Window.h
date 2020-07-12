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
typedef std::shared_ptr<Window> WndPtr;

enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

class Window {
public:
  ~Window();

  static void Init(const std::string &name, WindowMode wMode, ERROR &errCode);
  static void Init(const std::string &name, WindowMode wMode, int width, int height,
                   ERROR &errCode);

  static int GetWidth() { return GetInstance()->m_width; }
  static int GetHeight() { return GetInstance()->m_height; }
  static float GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); }
  static glm::vec2 RelToWinPos(const glm::vec2 &pos) {
    return {pos.x * GetWidth(), pos.y * GetHeight()};
  }
  static float GetDeltaX();
  static float GetDeltaY();

  static bool GetShouldClose() { return glfwWindowShouldClose(GetMainWindow()); }
  static void SwapBuffers() { glfwSwapBuffers(GetMainWindow()); }

  static const bool *GetKeys() { return GetInstance()->m_keys; }
  static bool GetToggleKey(int key, ERROR *errCode);

private:
  Window(); // prevent construction of this class

  static WndPtr GetInstance();
  static GLFWwindow *GetMainWindow() { return GetInstance()->m_mainWindow; }
  static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void MouseHandler(GLFWwindow *window, double xPos, double yPos);

  GLFWwindow *m_mainWindow;

  int m_width;
  int m_height;

  bool m_keys[NUM_KEYS];
  bool m_toggleKeys[NUM_KEYS];

  float m_lastX;
  float m_lastY;
  float m_deltaX;
  float m_deltaY;
  bool m_mouseFirstMoved;
};
