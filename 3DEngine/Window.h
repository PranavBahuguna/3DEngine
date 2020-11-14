#pragma once

// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// clang-format on

#include "Error.h"

#include <memory>
#include <string>

enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

class Window {
public:
  Window(const std::string &name, WindowMode wMode);
  Window(const std::string &name, WindowMode wMode, int width, int height);
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  ~Window();

  int getWidth() const;
  int getHeight() const;
  float getAspectRatio() const;
  glm::vec3 relToWinPos(const glm::vec2 &pos) const;
  bool getShouldClose() const;

  void swapBuffers();
  void close();

private:
  GLFWwindow *m_mainWindow;

  int m_width;
  int m_height;
};
