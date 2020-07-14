#include "Window.h"

#include <stdexcept>

static WndPtr _instance = nullptr;

// Constructor - zero all parameters
Window::Window()
    : m_width(0), m_height(0), m_lastX(0.0f), m_lastY(0.0f), m_deltaX(0.0f), m_deltaY(0.0f),
      m_offsetX(0.0f), m_offsetY(0.0f), m_mouseFirstMoved(true), m_mainWindow(nullptr) {

  std::fill_n(m_keys, NUM_KEYS, false);
  std::fill_n(m_toggleKeys, NUM_KEYS, true);
}

// Defaults width / height to 800/600 (applicable only if in windowed mode)
void Window::Init(const std::string &name, WindowMode wMode, ERROR &errCode) {
  Window::Init(name, wMode, 800, 600, errCode);
}

// Initialise all window properties
void Window::Init(const std::string &name, WindowMode wMode, int width, int height,
                  ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  if (_instance == nullptr)
    _instance = WndPtr(new Window());

  // Initialise GLFW
  if (glfwInit() != GLFW_TRUE) {
    errCode = ERROR_GLFW_INIT_FAILED;
    printErrorMsg(errCode);
    return;
  }

  // Setup GFLW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    errCode = ERROR_GLFW_WINDOW_CREATE_FAILED;
    printErrorMsg(errCode);
    return;
  }
  // Set context for GLEW to use
  glfwMakeContextCurrent(_instance->m_mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if (glewInit() != GLEW_OK) {
    errCode = ERROR_GLEW_INIT_FAILED;
    printErrorMsg(errCode);
    return;
  }

  // Setup viewport size
  glViewport(0, 0, _instance->m_width, _instance->m_height);
  // Setup keyboard and mouse handlers
  glfwSetKeyCallback(_instance->m_mainWindow, _instance->KeyHandler);
  glfwSetCursorPosCallback(_instance->m_mainWindow, _instance->MouseHandler);
  glfwSetScrollCallback(_instance->m_mainWindow, _instance->MouseScrollHandler);
  // Remove cursor from screen
  glfwSetInputMode(_instance->m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(_instance->m_mainWindow);
  glfwTerminate();
}

int Window::GetWidth() { return _instance->m_width; }

int Window::GetHeight() { return _instance->m_height; }

// Obtains and resets change in x-position
float Window::GetDeltaX() {
  float deltaX = _instance->m_deltaX;
  _instance->m_deltaX = 0.0f;
  return deltaX;
}

// Obtains and resets change in y-position
float Window::GetDeltaY() {
  float deltaY = _instance->m_deltaY;
  _instance->m_deltaY = 0.0f;
  return deltaY;
}

float Window::GetOffsetX() { return _instance->m_offsetX; }

float Window::GetOffsetY() { return _instance->m_offsetY; }

// Handles key input
void Window::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (action == GLFW_PRESS)
    _instance->m_keys[key] = true;
  if (action == GLFW_RELEASE)
    _instance->m_keys[key] = false;
}

// Handles mouse movement input
void Window::MouseHandler(GLFWwindow *window, double xPos, double yPos) {
  float fxPos = static_cast<float>(xPos);
  float fyPos = static_cast<float>(yPos);

  if (_instance->m_mouseFirstMoved) {
    _instance->m_lastX = fxPos;
    _instance->m_lastY = fyPos;
    _instance->m_mouseFirstMoved = false;
  }

  _instance->m_deltaX = fxPos - _instance->m_lastX;
  _instance->m_deltaY = _instance->m_lastY - fyPos;

  _instance->m_lastX = fxPos;
  _instance->m_lastY = fyPos;
}

// Handles mouse scroll input
void Window::MouseScrollHandler(GLFWwindow *window, double xOffset, double yOffset) {
  _instance->m_offsetX = static_cast<float>(xOffset);
  _instance->m_offsetY = static_cast<float>(yOffset);
}

const bool *Window::GetKeys() { return _instance->m_keys; }

// A toggle key can be queried during the rendering loop. It will return true the first time it is
// called if the corresponding key is pressed, but will return false on subsequent calls until the
// key is released and pressed again.
bool Window::GetToggleKey(int key, ERROR *errCode) {
  if (key < 0 || key >= NUM_KEYS) {
    *errCode = ERROR_INPUT_KEY_OUT_OF_RANGE;
    printErrorMsg(*errCode, key);
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
}

bool Window::GetShouldClose() { return glfwWindowShouldClose(_instance->m_mainWindow); }

void Window::SwapBuffers() { glfwSwapBuffers(_instance->m_mainWindow); }