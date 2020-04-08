#include <stdio.h>
#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main() {

  // Initialise GLFW
  if (!glfwInit()) {
    printf("GLFW initialisation failed");
    glfwTerminate();
    return EXIT_FAILURE;
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

  GLFWwindow *mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window", NULL, NULL);
  if (mainWindow == nullptr) {
    printf("GLFW window creation failed!");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if (glewInit() != GLEW_OK) {
    printf("GLEW initialisation failed!");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  // Loop until window closed
  while (!glfwWindowShouldClose(mainWindow))
  {
    // Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(mainWindow);
  }

  return EXIT_SUCCESS;
}
