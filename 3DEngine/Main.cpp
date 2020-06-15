#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include "Text.h"
#include "Window.h"

#include <iomanip>
#include <numeric>
#include <sstream>
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
#define COLOR_SEAWEED glm::vec4(0.0392f, 0.4941f, 0.549f, 1.0f)
#define COLOR_RED glm::vec4(0.651f, 0.1725f, 0.1686f, 1.0f)
#define COLOR_GREEN glm::vec4(0.1608f, 0.4314f, 0.0039f, 1.0f)
#define COLOR_BLUE glm::vec4(0.1961f, 0.3216f, 0.4824f, 1.0f)
#define COLOR_YELLOW glm::vec4(0.9922f, 0.80f, 0.051f, 1.0f)
#define COLOR_VIOLET glm::vec4(0.3569f, 0.0392f, 0.5686f, 1.0f)
#define FPS_UPDATE_DELAY 0.25f

ERROR errCode = ERROR_OK;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
std::vector<GLfloat> fpsCounts;
GLfloat fpsUpdateTime = FPS_UPDATE_DELAY;

bool displayHUD = false;

std::vector<Model *> modelList;
std::vector<Light *> sceneLights;
std::vector<Text *> textObjects;

// Converts a float to string with a number of decimal places
static std::string toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}

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

    Model *cube = new Cube;
    cube->setPosition(glm::vec3(-3.0f, 0.0f, 6.0f));
    cube->setRotation(glm::vec3(1.0f), 0.0f);
    cube->setScale(glm::vec3(0.4f));

    Model *earth = new Sphere;
    earth->setPosition(glm::vec3(3.0f, 0.0f, 6.0f));
    earth->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    earth->setScale(glm::vec3(0.4f));

    Model *starfighter = new Model("Arc170");
    starfighter->setPosition(glm::vec3(0.0f, 0.0f, 7.0f));
    starfighter->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    starfighter->setScale(glm::vec3(0.002f));

    modelList = {tetrahedron, cube, earth, starfighter};

    // Setup HUD elements
    const auto &dimensions = glm::vec2(window.getWidth(), window.getHeight());

    Text *fpsLabelText = new Text(HUD_FONT, {0.7f, 0.95f}, 1.0f, COLOR_SEAWEED, dimensions);
    fpsLabelText->setText("FPS:");
    Text *fpsValueText = new Text(HUD_FONT, {0.8f, 0.95f}, 1.0f, COLOR_SEAWEED, dimensions);

    Text *xPosLabelText = new Text(HUD_FONT, {0.7f, 0.85f}, 1.0f, COLOR_RED, dimensions);
    xPosLabelText->setText("X:");
    Text *xPosValueText = new Text(HUD_FONT, {0.8f, 0.85f}, 1.0f, COLOR_RED, dimensions);

    Text *yPosLabelText = new Text(HUD_FONT, {0.7f, 0.8f}, 1.0f, COLOR_GREEN, dimensions);
    yPosLabelText->setText("Y:");
    Text *yPosValueText = new Text(HUD_FONT, {0.8f, 0.8f}, 1.0f, COLOR_GREEN, dimensions);

    Text *zPosLabelText = new Text(HUD_FONT, {0.7f, 0.75f}, 1.0f, COLOR_BLUE, dimensions);
    zPosLabelText->setText("Z:");
    Text *zPosValueText = new Text(HUD_FONT, {0.8f, 0.75f}, 1.0f, COLOR_BLUE, dimensions);

    Text *pitchLabelText = new Text(HUD_FONT, {0.7f, 0.65f}, 1.0f, COLOR_YELLOW, dimensions);
    pitchLabelText->setText("Pitch:");
    Text *pitchValueText = new Text(HUD_FONT, {0.8f, 0.65f}, 1.0f, COLOR_YELLOW, dimensions);

    Text *yawLabelText = new Text(HUD_FONT, {0.7f, 0.6f}, 1.0f, COLOR_VIOLET, dimensions);
    yawLabelText->setText("Yaw:");
    Text *yawValueText = new Text(HUD_FONT, {0.8f, 0.6f}, 1.0f, COLOR_VIOLET, dimensions);

    textObjects = {fpsLabelText,   fpsValueText,   xPosLabelText, xPosValueText,
                   yPosLabelText,  yPosValueText,  zPosLabelText, zPosValueText,
                   pitchLabelText, pitchValueText, yawLabelText,  yawValueText};

    // Setup scene lights
    Light *light01 =
        new Light(glm::vec3(0.25f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(4.0f, 4.0f, -4.0f));

    sceneLights = {light01};

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
      fpsCounts.push_back(1.0f / deltaTime);

      // Handle user input events
      glfwPollEvents();
      camera.keyControl(window.getKeys(), deltaTime);
      camera.mouseControl(window.getDeltaX(), window.getDeltaY(), deltaTime);

      if (window.getToggleKey(GLFW_KEY_M, NULL))
        displayHUD = !displayHUD;

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
      if (displayHUD) {
        if (fpsUpdateTime >= FPS_UPDATE_DELAY) {
          GLfloat avgFps =
              std::accumulate(fpsCounts.begin(), fpsCounts.end(), 0.0f) / fpsCounts.size();
          fpsCounts.clear();
          fpsValueText->setText(toStringDp(avgFps, 1));
          fpsUpdateTime -= FPS_UPDATE_DELAY;
        } else {
          fpsUpdateTime += deltaTime;
        }

        const auto &cameraPos = camera.getPosition();

        xPosValueText->setText(toStringDp(cameraPos.x, 3));
        yPosValueText->setText(toStringDp(cameraPos.y, 3));
        zPosValueText->setText(toStringDp(cameraPos.z, 3));
        pitchValueText->setText(toStringDp(camera.getPitch(), 1));
        yawValueText->setText(toStringDp(camera.getYaw(), 1));

        for (auto it = textObjects.begin(); it != textObjects.end() && errCode == ERROR_OK; ++it)
          (*it)->draw(errCode);

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