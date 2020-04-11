#include "Window.h"

#include <stdexcept>

// Constructor - defaults to 800 x 600 size window
Window::Window() : Window("Test Window", 800, 600) {}

// Constructor
Window::Window(const std::string &name, GLint width, GLint height, bool useFullscreen)
    : m_name(name), m_width(width), m_height(height), m_useFullscreen(useFullscreen),
      m_mainWindow(nullptr), m_bufferWidth(0), m_bufferHeight(0) {

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

  return ERROR_OK;
}

// Destructor - destroy window and terminate
Window::~Window() {
  glfwDestroyWindow(m_mainWindow);
  glfwTerminate();
}