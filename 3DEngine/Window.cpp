#include "Window.h"

#include <stdexcept>

// Constructor
Window::Window(const std::string &name, WindowMode wMode, GLint width, GLint height)
    : m_name(name), m_wMode(wMode), m_width(width), m_height(height), m_lastX(0.0f), m_lastY(0.0f),
      m_mouseFirstMoved(true) {

  std::fill_n(m_toggleKeys, NUM_KEYS, true);
}

// Initialises the windows properties
void Window::initialize(ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

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
  if (m_wMode == WindowMode::FULLSCREEN || m_wMode == WindowMode::FULLSCREEN_WINDOWED) {
    auto primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    m_width = mode->width;
    m_height = mode->height;

    // Use the primary monitor only if we want fullscreen without window
    if (m_wMode == WindowMode::FULLSCREEN)
      monitor = primaryMonitor;
  }

  // Initialiase window with given dimension
  m_mainWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), monitor, NULL);
  if (m_mainWindow == nullptr) {
    errCode = ERROR_GLFW_WINDOW_CREATE_FAILED;
    printErrorMsg(errCode);
    return;
  }
  // Set context for GLEW to use
  glfwMakeContextCurrent(m_mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if (glewInit() != GLEW_OK) {
    errCode = ERROR_GLEW_INIT_FAILED;
    printErrorMsg(errCode);
    return;
  }

  // Setup viewport size
  glViewport(0, 0, m_width, m_height);

  // Setup keyboard and mouse handlers
  glfwSetKeyCallback(m_mainWindow, keyHandler);
  glfwSetCursorPosCallback(m_mainWindow, mouseHandler);
  // Remove cursor from screen
  glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetWindowUserPointer(m_mainWindow, this);
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(m_mainWindow);
  glfwTerminate();
}

// Obtains and resets change in x-position
GLfloat Window::getDeltaX() {
  GLfloat deltaX = m_deltaX;
  m_deltaX = 0.0f;
  return deltaX;
}

// Obtains and resets change in y-position
GLfloat Window::getDeltaY() {
  GLfloat deltaY = m_deltaY;
  m_deltaY = 0.0f;
  return deltaY;
}

// Handles key input
void Window::keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {

  Window *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (key < 0 || key >= NUM_KEYS) {
    printErrorMsg(ERROR_INPUT_KEY_OUT_OF_RANGE, key);
    return;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (action == GLFW_PRESS)
    thisWindow->m_keys[key] = true;
  if (action == GLFW_RELEASE)
    thisWindow->m_keys[key] = false;
}

// Handles mouse input
void Window::mouseHandler(GLFWwindow *window, double xPos, double yPos) {

  Window *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

  GLfloat fxPos = static_cast<GLfloat>(xPos);
  GLfloat fyPos = static_cast<GLfloat>(yPos);

  if (thisWindow->m_mouseFirstMoved) {
    thisWindow->m_lastX = fxPos;
    thisWindow->m_lastY = fyPos;
    thisWindow->m_mouseFirstMoved = false;
  }

  thisWindow->m_deltaX = fxPos - thisWindow->m_lastX;
  thisWindow->m_deltaY = thisWindow->m_lastY - fyPos;

  thisWindow->m_lastX = fxPos;
  thisWindow->m_lastY = fyPos;
}

// A toggle key can be queried during the rendering loop. It will return true the first time it is
// called if the corresponding key is pressed, but will return false on subsequent calls until the
// key is released and pressed again.
bool Window::getToggleKey(int key, ERROR *errCode) {
  if (key < 0 || key >= NUM_KEYS) {
    *errCode = ERROR_INPUT_KEY_OUT_OF_RANGE;
    printErrorMsg(*errCode, key);
    return false;
  }

  if (m_keys[key]) {
    if (m_toggleKeys[key]) {
      m_toggleKeys[key] = false;
      return true;
    }
  } else {
    m_toggleKeys[key] = true;
  }

  return false;
}