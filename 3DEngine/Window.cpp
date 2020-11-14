#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <stdexcept>

// Defaults width / height to 800/600 (applicable only if in windowed mode)
Window::Window(const std::string &name, WindowMode wMode) : Window(name, wMode, 800, 600) {}

Window::Window(const std::string &name, WindowMode wMode, int width, int height)
    : m_width(width), m_height(height), m_mainWindow(nullptr) {

  // Initialise GLFW
  ERROR errCode = ERROR_OK;
  if (glfwInit() != GLFW_TRUE) {
    errCode = printErrorMsg(ERROR_GLFW_INIT_FAILED);
  } else {
    // Setup GFLW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forwards compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Set window dimensions from screen if fullscreen enabled
    GLFWmonitor *monitor = nullptr;
    if (wMode == WindowMode::FULLSCREEN || wMode == WindowMode::FULLSCREEN_WINDOWED) {
      auto primaryMonitor = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
      m_width = mode->width;
      m_height = mode->height;

      // Use the primary monitor only if we want fullscreen without window
      if (wMode == WindowMode::FULLSCREEN)
        monitor = primaryMonitor;
    }

    // Initialiase window with given dimension
    m_mainWindow = glfwCreateWindow(m_width, m_height, name.c_str(), monitor, NULL);
    if (m_mainWindow == nullptr) {
      errCode = printErrorMsg(ERROR_GLFW_WINDOW_CREATE_FAILED);
    } else {
      // Set context for GLEW to use
      glfwMakeContextCurrent(m_mainWindow);

      // Allow modern extension features
      glewExperimental = GL_TRUE;

      // Initialise GLEW
      if (glewInit() != GLEW_OK) {
        errCode = printErrorMsg(ERROR_GLEW_INIT_FAILED);
      } else {
        // Setup viewport size
        glViewport(0, 0, m_width, m_height);
        // Setup keyboard and mouse handlers
        glfwSetKeyCallback(m_mainWindow, Keyboard::KeyHandler);
        glfwSetCursorPosCallback(m_mainWindow, Mouse::MouseHandler);
        glfwSetScrollCallback(m_mainWindow, Mouse::MouseScrollHandler);
        // Remove cursor from screen
        glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    }
  }

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while initialising window.");
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(m_mainWindow);
  glfwTerminate();
}

int Window::getWidth() const { return m_width; }

int Window::getHeight() const { return m_height; }

float Window::getAspectRatio() const { return (float)m_width / (float)m_height; }

glm::vec3 Window::relToWinPos(const glm::vec2 &pos) const {
  return {pos.x * m_width, pos.y * m_height, 0.0f};
}

bool Window::getShouldClose() const { return glfwWindowShouldClose(m_mainWindow); }

void Window::swapBuffers() { glfwSwapBuffers(m_mainWindow); }

void Window::close() { glfwSetWindowShouldClose(m_mainWindow, GLFW_TRUE); }