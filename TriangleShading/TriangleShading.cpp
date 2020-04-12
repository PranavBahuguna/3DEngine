#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#ifdef DEBUG
#define USE_FULLSCREEN false
#else
#define USE_FULLSCREEN true
#endif
#define CAMERA_MOVE_SPEED 0.1f
#define CAMERA_TURN_SPEED 1.0f
#define FOV 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

std::vector<Mesh *> meshList;

glm::vec3 triPos(0.0f, 0.0f, 3.0f);
glm::vec3 triScale(0.4f);
glm::vec3 triRotation(1.0f);
float triCurAngle = 0.0f;

glm::vec3 cameraPos(0.0f);

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
  // Handle rotation
  triCurAngle += 0.1f;
  if (triCurAngle >= 360.0f)
    triCurAngle -= 360.0f;
}

int main() {
  try {
    // Create a fullscreen window
    Window window("Test window", WINDOW_WIDTH, WINDOW_HEIGHT, USE_FULLSCREEN);

    // Create the triangle in the scene
    CreateTriangle();

    // Load and compile the triangle shader
    Shader triangleShader("Triangle");

    // Get MVP parameter from shader
    GLint uMVP = triangleShader.getUL("mvp");

    // Setup camera projection
    Camera camera(cameraPos, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, CAMERA_MOVE_SPEED,
                  CAMERA_TURN_SPEED);
    glm::mat4 projection = glm::perspective(FOV, window.getAspectRatio(), NEAR_PLANE, FAR_PLANE);

    // Main program loop
    while (!window.getShouldClose()) {
      // Handle user input events
      glfwPollEvents();
      camera.keyControl(window.getKeys());

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update triangle motion
      UpdateMotion();

      // Apply transforms to the model
      glm::mat4 model(1.0f);
      model = glm::translate(model, triPos);
      model = glm::rotate(model, glm::radians(triCurAngle), triRotation);
      model = glm::scale(model, triScale);

      // Get camera view
      glm::mat4 view = camera.calcViewMatrix();

      // Calculate the MVP matrix and apply to shader
      glm::mat4 mvp = projection * view * model;
      glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mvp));

      // Draw all meshes
      for (const auto &mesh : meshList)
        mesh->Render();

      triangleShader.use();

      window.swapBuffers();
    }
  } catch (std::exception e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
