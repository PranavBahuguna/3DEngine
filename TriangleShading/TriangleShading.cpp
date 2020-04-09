#include "Shader.h"
#include "Triangle.h"

#include <Windows.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
float triCurAngle = 0.0f;
float triScale = 0.4f;

// Handle user input
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Updates the motion of a triangle
void UpdateMotion(Triangle triangle) {
  
  // Handle left to right motion
  triOffset += direction ? triIncrement : -triIncrement;
  if (abs(triOffset) >= triMaxOffset)
    direction = !direction;

  // Handle rotation
  triCurAngle += 0.1f;
  if (triCurAngle >= 360.0f)
    triCurAngle -= 360.0f;
}

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

  GLFWwindow *mainWindow =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window", glfwGetPrimaryMonitor(), NULL);
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

  // Setup key callback
  glfwSetKeyCallback(mainWindow, key_callback);

  // Create the triangle in the scene
  Triangle triangle;

  // Load and compile the triangle shader
  Shader triangleShader("Triangle");

  // Get model and projection
  GLint uModel = triangleShader.GetUL("model");
  GLint uProjection = triangleShader.GetUL("projection");

  // Setup projection
  glm::mat4 projection =
      glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

  // Loop until window closed
  while (!glfwWindowShouldClose(mainWindow)) {
    // Handle user input events
    glfwPollEvents();

    UpdateMotion(triangle);

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    triangleShader.Use();

    // Apply transforms to the model
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    model = glm::rotate(model, glm::radians(triCurAngle), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(triScale));

    // Set model and projection
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the triangle
    triangle.Draw();
    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
  }

  return EXIT_SUCCESS;
}
