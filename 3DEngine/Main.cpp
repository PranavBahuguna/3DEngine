#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include "Text.h"
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
#define HUD_FONT "Unreal"

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

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    Model *starfighter = new Model("Arc170");
    starfighter->setPosition(glm::vec3(0.0f, 0.0f, 7.0f));
    starfighter->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    starfighter->setScale(glm::vec3(0.002f));
    modelList.push_back(starfighter);

    // Setup HUD elements
    const auto &windowDimensions = glm::vec2(window.getWidth(), window.getHeight());
    const auto &red = glm::vec4(0.651f, 0.1725f, 0.1686f, 1.0f);
    const auto &green = glm::vec4(0.1608f, 0.4314f, 0.0039f, 1.0f);
    const auto &blue = glm::vec4(0.1961f, 0.3216f, 0.4824f, 1.0f);
    const auto &yellow = glm::vec4(0.9922f, 0.80f, 0.051f, 1.0f);
    const auto &violet = glm::vec4(0.3569f, 0.0392f, 0.5686f, 1.0f);

    Text xPosText(HUD_FONT, glm::vec2(0.7f, 0.9f), 1.0f, red, windowDimensions);
    Text yPosText(HUD_FONT, glm::vec2(0.7f, 0.85f), 1.0f, green, windowDimensions);
    Text zPosText(HUD_FONT, glm::vec2(0.7f, 0.8f), 1.0f, blue, windowDimensions);
    Text pitchText(HUD_FONT, glm::vec2(0.7f, 0.7f), 1.0f, yellow, windowDimensions);
    Text yawText(HUD_FONT, glm::vec2(0.7f, 0.65f), 1.0f, violet, windowDimensions);

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
          model->applyLight(*light, errCode);

        model->draw(camera, errCode);

        if (errCode != ERROR_OK)
          throw std::runtime_error("An error occurred while processing model " +
                                   std::to_string(model->_id) + ".");
      }

      // Draw HUD elements to screen
      if (camera.isHUDEnabled()) {
        const auto &cameraPos = camera.getPosition();

        xPosText.setText("X:     " + std::to_string(cameraPos.x));
        xPosText.draw(errCode);
        yPosText.setText("Y:     " + std::to_string(cameraPos.y));
        yPosText.draw(errCode);
        zPosText.setText("Z:     " + std::to_string(cameraPos.z));
        zPosText.draw(errCode);
        pitchText.setText("Pitch: " + std::to_string(camera.getPitch()));
        pitchText.draw(errCode);
        yawText.setText("Yaw:   " + std::to_string(camera.getYaw()));
        yawText.draw(errCode);

        if (errCode != ERROR_OK)
          throw std::runtime_error("An error occurred while drawing HUD element.");
      }

      window.swapBuffers();
    }
  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
