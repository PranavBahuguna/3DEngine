#include "Window.h"

#include <stdexcept>

// Constructor - defaults to 800 x 600 size window
Window::Window() : Window("Test Window", 800, 600) {}

// Constructor
Window::Window(const std::string &name, GLint width, GLint height, bool useFullscreen)
    : m_name(name), m_width(width), m_height(height), m_useFullscreen(useFullscreen), m_lastX(0.0f),
      m_lastY(0.0f), m_mouseFirstMoved(true) {

  ERROR errCode = initialize();
  if (errCode != ERROR_OK) {
    printErrorMsg(errCode);
    throw std::runtime_error("An error occurred during shader construction.");
  }
}

// Initialises the windows properties
ERROR Window::initialize() {
  // Initialise GLFW
  if (glfwInit() != GLFW_TRUE)
    return ERROR_GLFW_INIT_FAILED;

  // Setup GFLW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Core profile - no backwards compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forwards compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Get primary monitor if fullscreen is enabled
  GLFWmonitor *monitor = m_useFullscreen ? glfwGetPrimaryMonitor() : nullptr;

  m_mainWindow = glfwCreateWindow(m_width, m_height, "Test Window", monitor, NULL);
  if (m_mainWindow == nullptr)
    return ERROR_GLFW_WINDOW_CREATE_FAILED;

  // Get buffer size information
  glfwGetFramebufferSize(m_mainWindow, &m_bufferWidth, &m_bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(m_mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if (glewInit() != GLEW_OK)
    return ERROR_GLEW_INIT_FAILED;

  // Setup viewport size
  glViewport(0, 0, m_bufferWidth, m_bufferHeight);

  // Setup keyboard and mouse handlers
  glfwSetKeyCallback(m_mainWindow, keyHandler);
  glfwSetCursorPosCallback(m_mainWindow, mouseHandler);
  // Remove cursor from screen
  glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetWindowUserPointer(m_mainWindow, this);

  return ERROR_OK;
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

// Obtains and resets change in x-position
GLfloat Window::getDeltaY() {
  GLfloat deltaY = m_deltaY;
  m_deltaY = 0.0f;
  return deltaY;
}

// Handles key input
void Window::keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {

  Window *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      thisWindow->m_keys[key] = true;
    if (action == GLFW_RELEASE)
      thisWindow->m_keys[key] = false;
  }
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
