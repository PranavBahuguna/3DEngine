#include "Camera.h"
#include "Tetrahedron.h"
#include "Window.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
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

typedef std::vector<Object *> ObjList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int main() {
  try {
    // Create a fullscreen window
    Window window("Test window", WINDOW_WIDTH, WINDOW_HEIGHT, USE_FULLSCREEN);

    // Create a tetrahedron in the scene and add to the object list
    ObjList objList;
    Tetrahedron *t1 = new Tetrahedron;
    t1->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    t1->setRotation(0.0f, glm::vec3(1.0f));
    t1->setScale(glm::vec3(0.4f));
    objList.push_back(t1);

    // Setup camera
    Camera camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, CAMERA_MOVE_SPEED,
                  CAMERA_TURN_SPEED);
    camera.setProjection(FOV, window.getAspectRatio(), NEAR_PLANE, FAR_PLANE);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

      // Update triangle motion
      for (auto &obj : objList) {
        if (obj->update() != ERROR_OK)
          throw std::runtime_error(
              "An error occurred while updating object (id = " + std::to_string(obj->_id) + ")");

        if (obj->draw(camera) != ERROR_OK)
          throw std::runtime_error(
              "An error occurred while drawing object (id = " + std::to_string(obj->_id) + ")");
      }

      window.swapBuffers();
    }
  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
