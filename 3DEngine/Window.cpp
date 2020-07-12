#include "Window.h"

#include <stdexcept>

static WndPtr instance = nullptr;

// Constructor - zero all parameters
Window::Window()
    : m_width(0), m_height(0), m_lastX(0.0f), m_lastY(0.0f), m_deltaX(0.0f), m_deltaY(0.0f),
      m_mouseFirstMoved(true), m_mainWindow(nullptr) {

  std::fill_n(m_keys, NUM_KEYS, false);
  std::fill_n(m_toggleKeys, NUM_KEYS, true);
}

// Get window singleton instance
WndPtr Window::GetInstance() {
  if (instance == nullptr)
    instance = WndPtr(new Window());

  return instance;
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

  if (instance == nullptr)
    instance = GetInstance();

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
    instance->m_width = mode->width;
    instance->m_height = mode->height;

    // Use the primary monitor only if we want fullscreen without window
    if (wMode == WindowMode::FULLSCREEN)
      monitor = primaryMonitor;
  } else {
    instance->m_height = height;
    instance->m_width = width;
  }

  // Initialiase window with given dimension
  instance->m_mainWindow =
      glfwCreateWindow(instance->m_width, instance->m_height, name.c_str(), monitor, NULL);
  if (instance->m_mainWindow == nullptr) {
    errCode = ERROR_GLFW_WINDOW_CREATE_FAILED;
    printErrorMsg(errCode);
    return;
  }
  // Set context for GLEW to use
  glfwMakeContextCurrent(instance->m_mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if (glewInit() != GLEW_OK) {
    errCode = ERROR_GLEW_INIT_FAILED;
    printErrorMsg(errCode);
    return;
  }

  // Setup viewport size
  glViewport(0, 0, instance->m_width, instance->m_height);

  // Setup keyboard and mouse handlers
  glfwSetKeyCallback(instance->m_mainWindow, instance->KeyHandler);
  glfwSetCursorPosCallback(instance->m_mainWindow, instance->MouseHandler);
  // Remove cursor from screen
  glfwSetInputMode(instance->m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glfwSetWindowUserPointer(instance->m_mainWindow, &instance);
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(instance->m_mainWindow);
  glfwTerminate();
}

// Obtains and resets change in x-position
float Window::GetDeltaX() {
  WndPtr wnd = GetInstance();
  float deltaX = wnd->m_deltaX;
  wnd->m_deltaX = 0.0f;
  return deltaX;
}

// Obtains and resets change in y-position
float Window::GetDeltaY() {
  WndPtr wnd = GetInstance();
  float deltaY = wnd->m_deltaY;
  wnd->m_deltaY = 0.0f;
  return deltaY;
}

// Handles key input
void Window::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
  WndPtr wnd = GetInstance();

  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (action == GLFW_PRESS)
    wnd->m_keys[key] = true;
  if (action == GLFW_RELEASE)
    wnd->m_keys[key] = false;
}

// Handles mouse input
void Window::MouseHandler(GLFWwindow *window, double xPos, double yPos) {
  WndPtr wnd = GetInstance();

  float fxPos = static_cast<float>(xPos);
  float fyPos = static_cast<float>(yPos);

  if (wnd->m_mouseFirstMoved) {
    wnd->m_lastX = fxPos;
    wnd->m_lastY = fyPos;
    wnd->m_mouseFirstMoved = false;
  }

  wnd->m_deltaX = fxPos - wnd->m_lastX;
  wnd->m_deltaY = wnd->m_lastY - fyPos;

  wnd->m_lastX = fxPos;
  wnd->m_lastY = fyPos;
}

// A toggle key can be queried during the rendering loop. It will return true the first time it is
// called if the corresponding key is pressed, but will return false on subsequent calls until the
// key is released and pressed again.
bool Window::GetToggleKey(int key, ERROR *errCode) {
  if (key < 0 || key >= NUM_KEYS) {
    *errCode = ERROR_INPUT_KEY_OUT_OF_RANGE;
    printErrorMsg(*errCode, key);
    return false;
  }

  WndPtr wnd = GetInstance();

  if (wnd->m_keys[key]) {
    if (wnd->m_toggleKeys[key]) {
      wnd->m_toggleKeys[key] = false;
      return true;
    }
  } else {
    wnd->m_toggleKeys[key] = true;
  }

  return false;
}