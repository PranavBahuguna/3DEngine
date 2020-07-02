#pragma once

#include "Error.h"

#include <string>
// clang-format off
#include <GL\glew.h>
#include <GLFW\glfw3.h>
// clang-format on

#define NUM_KEYS 512

enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

class Window {
public:
  Window(const std::string &name, WindowMode wMode = WindowMode::WINDOWED, GLint width = 800,
         GLint height = 600);
  ~Window();

  void initialize(ERROR &errCode);

  GLint getWidth() const { return m_width; }
  GLint getHeight() const { return m_height; }
  GLfloat getAspectRatio() const { return (GLfloat)m_width / (GLfloat)m_height; }
  bool getShouldClose() const { return glfwWindowShouldClose(m_mainWindow); }
  void swapBuffers() const { glfwSwapBuffers(m_mainWindow); }

  GLfloat getDeltaX();
  GLfloat getDeltaY();

  const bool *getKeys() const { return m_keys; }
  bool getToggleKey(int key, ERROR *errCode);

private:
  static void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseHandler(GLFWwindow *window, double xPos, double yPos);

  GLFWwindow *m_mainWindow;
  std::string m_name;
  WindowMode m_wMode;

  GLint m_width;
  GLint m_height;

  bool m_keys[NUM_KEYS];
  bool m_toggleKeys[NUM_KEYS];

  GLfloat m_lastX;
  GLfloat m_lastY;
  GLfloat m_deltaX;
  GLfloat m_deltaY;
  bool m_mouseFirstMoved;
};
