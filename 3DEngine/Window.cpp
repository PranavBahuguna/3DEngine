#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <stdexcept>

static WndPtr _instance = nullptr;

// Constructor - zero all parameters
Window::Window()
    : m_width(0), m_height(0), m_mainWindow(nullptr) {}

// Defaults width / height to 800/600 (applicable only if in windowed mode)
void Window::Init(const std::string &name, WindowMode wMode) {
  Window::Init(name, wMode, 800, 600);
}

// Initialise all window properties
void Window::Init(const std::string &name, WindowMode wMode, int width, int height) {
  if (_instance == nullptr)
    _instance = WndPtr(new Window());

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
      _instance->m_width = mode->width;
      _instance->m_height = mode->height;

      // Use the primary monitor only if we want fullscreen without window
      if (wMode == WindowMode::FULLSCREEN)
        monitor = primaryMonitor;
    } else {
      _instance->m_height = height;
      _instance->m_width = width;
    }

    // Initialiase window with given dimension
    _instance->m_mainWindow =
        glfwCreateWindow(_instance->m_width, _instance->m_height, name.c_str(), monitor, NULL);
    if (_instance->m_mainWindow == nullptr) {
      errCode = printErrorMsg(ERROR_GLFW_WINDOW_CREATE_FAILED);
    } else {
      // Set context for GLEW to use
      glfwMakeContextCurrent(_instance->m_mainWindow);

      // Allow modern extension features
      glewExperimental = GL_TRUE;

      // Initialise GLEW
      if (glewInit() != GLEW_OK) {
        errCode = printErrorMsg(ERROR_GLEW_INIT_FAILED);
      } else {
        // Setup viewport size
        glViewport(0, 0, _instance->m_width, _instance->m_height);
        // Setup keyboard and mouse handlers
        glfwSetKeyCallback(_instance->m_mainWindow, Keyboard::KeyHandler);
        glfwSetCursorPosCallback(_instance->m_mainWindow, Mouse::MouseHandler);
        glfwSetScrollCallback(_instance->m_mainWindow, Mouse::MouseScrollHandler);
        // Remove cursor from screen
        glfwSetInputMode(_instance->m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    }
  }

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while initialising window.");
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(_instance->m_mainWindow);
  glfwTerminate();
}

int Window::GetWidth() { return _instance->m_width; }

int Window::GetHeight() { return _instance->m_height; }

float Window::GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); }

glm::vec2 Window::RelToWinPos(const glm::vec2 &pos) {
  return {pos.x * GetWidth(), pos.y * GetHeight()};
}

/*
// A toggle key can be queried during the rendering loop. It will return true the first time it is
// called if the corresponding key is pressed, but will return false on subsequent calls until the
// key is released and pressed again.
bool Window::GetToggleKey(ERROR &errCode, int key) {
  if (key < 0 || key >= NUM_KEYS) {
    errCode = printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return false;
  }

  if (_instance->m_keys[key]) {
    if (_instance->m_toggleKeys[key]) {
      _instance->m_toggleKeys[key] = false;
      return true;
    }
  } else {
    _instance->m_toggleKeys[key] = true;
  }

  return false;
}*/

bool Window::GetShouldClose() { return glfwWindowShouldClose(_instance->m_mainWindow); }

void Window::SwapBuffers() { glfwSwapBuffers(_instance->m_mainWindow); }