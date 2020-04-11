#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<Mesh *> meshList;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
float triCurAngle = 0.0f;
float triScale = 0.4f;

// Creates a triangle and adds it to the mesh list
void CreateTriangle() {

  // Initialise vertices and indices
  GLfloat height = sqrt(3.0f);

  std::vector<GLfloat> vertices = {
      -1.0f, 0.0f,   -1.0f, // front-left
      0.0f,  height, 0.0f,  // top
      1.0f,  0.0f,   -1.0f, // front-right
      0.0f,  0.0f,   1.0f,  // back
  };

  std::vector<unsigned int> indices = {
      0, 3, 1, // back-left
      1, 3, 2, // back-right
      2, 3, 0, // bottom
      0, 1, 2  // front
  };

  // Add to the mesh list
  meshList.push_back(new Mesh(vertices, indices));
}

// Updates the motion of a triangle
void UpdateMotion() {

  // Handle left to right motion
  triOffset += direction ? triIncrement : -triIncrement;
  if (abs(triOffset) >= triMaxOffset)
    direction = !direction;

  // Handle rotation
  triCurAngle += 0.1f;
  if (triCurAngle >= 360.0f)
    triCurAngle -= 360.0f;
}

// Handle user input
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {

  // Create a fullscreen window
  Window window("Test window", 800, 600, false);
  window.setKeyCallback(keyCallback);

  // Create the triangle in the scene
  CreateTriangle();

  // Load and compile the triangle shader
  Shader triangleShader("Triangle");

  // Get MVP parameter from shader
  GLint uMVP = triangleShader.getUL("mvp");

  // Setup camera projection and view
  glm::mat4 projection = glm::perspective(45.0f, window.getAspectRatio(), 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));

  // Main program loop
  while (!window.getShouldClose()) {
    // Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update triangle motion
    UpdateMotion();

    // Apply transforms to the model
    glm::mat4 model(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.0f));
    model = glm::rotate(model, glm::radians(triCurAngle), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(triScale));

    // Calculate the MVP matrix and apply to shader
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mvp));

    // Draw all meshes
    for (const auto &mesh : meshList)
      mesh->Render();

    triangleShader.use();

    window.swapBuffers();
  }

  return EXIT_SUCCESS;
}
