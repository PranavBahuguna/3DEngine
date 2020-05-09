#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include "Window.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200
#ifdef _DEBUG
#define USE_FULLSCREEN false
#else
#define USE_FULLSCREEN true
#endif
#define CAMERA_MOVE_SPEED 5.0f
#define CAMERA_TURN_SPEED 50.0f
#define FOV 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

ERROR errCode = ERROR_OK;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<Model *> modelList;
std::vector<Light *> sceneLights;

int main() {
  try {
    // Create a fullscreen window
    Window window("Test window", WINDOW_WIDTH, WINDOW_HEIGHT, USE_FULLSCREEN);

    // Allow objects to obscure other objects behind them
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Setup scene objects
    Model *tetrahedron = new Tetrahedron;
    tetrahedron->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    tetrahedron->setRotation(glm::vec3(1.0f), 0.0f);
    tetrahedron->setScale(glm::vec3(0.4f));
    modelList.push_back(tetrahedron);

    Model *cube = new Cube;
    cube->setPosition(glm::vec3(-3.0f, 0.0f, 6.0f));
    cube->setRotation(glm::vec3(1.0f), 0.0f);
    cube->setScale(glm::vec3(0.4f));
    modelList.push_back(cube);

    Model *earth = new Sphere;
    earth->setPosition(glm::vec3(3.0f, 0.0f, 6.0f));
    earth->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    earth->setScale(glm::vec3(0.4f));
    modelList.push_back(earth);

    // Setup scene lights
    Light *light01 =
        new Light(glm::vec3(0.25f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(4.0f, 4.0f, -4.0f));
    sceneLights.push_back(light01);

    // Setup camera
    Camera camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, CAMERA_MOVE_SPEED,
                  CAMERA_TURN_SPEED);
    camera.setProjection(FOV, window.getAspectRatio(), NEAR_PLANE, FAR_PLANE);

    // Main program loop
    while (!window.getShouldClose()) {
      // Get time elapsed since last cycle
      GLfloat timeNow = static_cast<GLfloat>(glfwGetTime());
      deltaTime = timeNow - lastTime;
      lastTime = timeNow;

      // Handle user input events
      glfwPollEvents();
      camera.keyControl(window.getKeys(), deltaTime);
      camera.mouseControl(window.getDeltaX(), window.getDeltaY(), deltaTime);

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update, light and draw each model
      for (const auto &model : modelList) {
        model->update(errCode);

        for (const auto &light : sceneLights)
          model->applyLight(*light);

        model->draw(camera, errCode);

        if (errCode != ERROR_OK)
          throw std::runtime_error("An error occurred while processing model " +
                                   std::to_string(model->_id) + ".");
      }

      window.swapBuffers();
    }
  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
