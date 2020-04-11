#pragma once

#include "Error.h"

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window {
public:
  Window();
  Window(const std::string &name, GLint width, GLint height, bool useFullscreen = false);
  ~Window();

  GLfloat getBufferWidth() const { return (GLfloat)m_bufferWidth; }
  GLfloat getBufferHeight() const { return (GLfloat)m_bufferHeight; }
  GLfloat getAspectRatio() const { return getBufferWidth() / getBufferHeight(); }

  bool getShouldClose() const { return glfwWindowShouldClose(m_mainWindow); }
  void setKeyCallback(GLFWkeyfun callback) const { glfwSetKeyCallback(m_mainWindow, callback); }
  void swapBuffers() const { glfwSwapBuffers(m_mainWindow); }

private:
  ERROR initialize();

  GLFWwindow *m_mainWindow;
  std::string m_name;
  bool m_useFullscreen;

  GLint m_width;
  GLint m_height;
  GLint m_bufferWidth;
  GLint m_bufferHeight;
};
