#pragma once

#include "Error.h"

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define NUM_KEYS 512

class Window {
public:
  Window();
  Window(const std::string &name, GLint width, GLint height, bool useFullscreen = false);
  ~Window();

  GLint getWidth() { return m_width; }
  GLint getHeight() { return m_height; }
  GLfloat getAspectRatio() const { return (GLfloat)m_width / (GLfloat)m_height; }
  bool getShouldClose() const { return glfwWindowShouldClose(m_mainWindow); }
  void swapBuffers() const { glfwSwapBuffers(m_mainWindow); }

  GLfloat getDeltaX();
  GLfloat getDeltaY();

  const bool* getKeys() const { return m_keys; }
  bool getToggleKey(int key, ERROR *errCode);

private:
  ERROR initialize();

  static void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseHandler(GLFWwindow *window, double xPos, double yPos);

  GLFWwindow *m_mainWindow;
  std::string m_name;
  bool m_useFullscreen;

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
