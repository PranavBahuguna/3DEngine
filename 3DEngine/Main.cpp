#define TINYOBJLOADER_IMPLEMENTATION

#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "LightIcon.h"
#include "Sphere.h"
#include "Terrain.h"
#include "Tetrahedron.h"
#include "Text.h"
#include "Window.h"

#include <algorithm>
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
#define USE_WINDOWED true
#else
#define USE_WINDOWED false
#endif
#define FULLSCREEN_WINDOWS false
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
#define FPS_UPDATE_DELAY 0.5f
#define FPS_BUFFER_SIZE 8
#define MAX_LIGHTS 8

ERROR errCode = ERROR_OK;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<GLfloat> fpsBuffer(FPS_BUFFER_SIZE);
size_t fpsBufferIdx = 0;
GLfloat fpsUpdateTime = FPS_UPDATE_DELAY;

bool displayHUD = false;

std::vector<Model *> modelList;
std::vector<LightPtr> sceneLights;
std::vector<Text *> textObjects;
std::vector<LightIcon *> lightIcons;

size_t nLights = 0;

Window *window;

// Converts a float to string with a number of decimal places
static std::string toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}

// Converts from a relative position to screen position
static glm::vec2 relToScreenPos(const glm::vec2 &pos) {
  return {pos.x * window->getWidth(), pos.y * window->getHeight()};
}

int main() {
  try {
    // Determine the window mode to use and create a window
    WindowMode wMode = (!USE_WINDOWED) ? WindowMode::FULLSCREEN
                                       : (FULLSCREEN_WINDOWS) ? WindowMode::FULLSCREEN_WINDOWED
                                                              : WindowMode::WINDOWED;

    window = new Window("Test window", wMode, WINDOW_WIDTH, WINDOW_HEIGHT);
    window->initialize(errCode);

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
    tetrahedron->setScale(glm::vec3(0.4f));

    Model *cube = new Cube;
    cube->setPosition(glm::vec3(-3.0f, 0.0f, 6.0f));
    cube->setScale(glm::vec3(0.4f));

    Model *earth = new Sphere;
    earth->setPosition(glm::vec3(3.0f, 0.0f, 6.0f));
    earth->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    earth->setScale(glm::vec3(0.4f));

    Model *starfighter = new Model("Arc170");
    starfighter->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    starfighter->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    starfighter->setScale(glm::vec3(0.002f));

    Terrain *floor = new Terrain("Grass", {5, 5}, {5.0f, 5.0f});
    floor->setPosition(glm::vec3(0.0f, -3.0f, 0.0f));
    floor->setRotation(glm::vec3(1.0f), 0.0f);

    modelList = {tetrahedron, cube, earth, starfighter, floor};

    for (auto it = modelList.begin(); it != modelList.end() && errCode == ERROR_OK; it++)
      (*it)->load(errCode);

    // Setup HUD elements
    Text *fpsLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.95f}), 1.0f, COLOR_SEAWEED);
    fpsLabelText->setText("FPS:");
    Text *fpsValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.95f}), 1.0f, COLOR_SEAWEED);

    Text *xPosLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.85f}), 1.0f, COLOR_RED);
    xPosLabelText->setText("X:");
    Text *xPosValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.85f}), 1.0f, COLOR_RED);

    Text *yPosLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.8f}), 1.0f, COLOR_GREEN);
    yPosLabelText->setText("Y:");
    Text *yPosValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.8f}), 1.0f, COLOR_GREEN);

    Text *zPosLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.75f}), 1.0f, COLOR_BLUE);
    zPosLabelText->setText("Z:");
    Text *zPosValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.75f}), 1.0f, COLOR_BLUE);

    Text *pitchLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.65f}), 1.0f, COLOR_YELLOW);
    pitchLabelText->setText("Pitch:");
    Text *pitchValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.65f}), 1.0f, COLOR_YELLOW);

    Text *yawLabelText = new Text(HUD_FONT, relToScreenPos({0.7f, 0.6f}), 1.0f, COLOR_VIOLET);
    yawLabelText->setText("Yaw:");
    Text *yawValueText = new Text(HUD_FONT, relToScreenPos({0.8f, 0.6f}), 1.0f, COLOR_VIOLET);

    textObjects = {fpsLabelText,   fpsValueText,   xPosLabelText, xPosValueText,
                   yPosLabelText,  yPosValueText,  zPosLabelText, zPosValueText,
                   pitchLabelText, pitchValueText, yawLabelText,  yawValueText};

    // Setup scene lights
    LightPtr light01 =
        Resources::CreateDirectionalLight("DirectionalLight", glm::vec3(0.25f), glm::vec3(1.0f),
                                          glm::vec3(1.0f), {1.0f, 1.0f, -1.0f});
    LightPtr light02 = Resources::CreatePointLight(
        "PointLight", glm::vec3(0.25f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f),
        glm::vec3(4.0f, 4.0f, -4.0f), 1.0f, 0.045f, 0.0075f);
    LightPtr light03 = Resources::CreateSpotLight(
        "SpotLight", glm::vec3(0.25f), glm::vec3(1.0f), glm::vec3(1.0f), {-4.0f, 10.0f, 3.0f}, 1.0f,
        0.045f, 0.0075f, {0.0f, -1.0f, 0.0f}, 20.0f, 25.0f);

    sceneLights = {light01, light02, light03};
    nLights = std::min(sceneLights.size(), (size_t)MAX_LIGHTS);

    // Setup light icons
    LightIcon *li01 = new LightIcon("DirectionalLight");
    LightIcon *li02 = new LightIcon("PointLight");
    LightIcon *li03 = new LightIcon("SpotLight");

    lightIcons = {li01, li02, li03};

    // Setup lighting shader
    auto lightingShader = Resources::GetShader("Lighting");
    lightingShader->setPreprocessor(GL_FRAGMENT_SHADER, "MAX_LIGHTS", MAX_LIGHTS);
    lightingShader->compile(errCode);
    lightingShader->setInt("nLights", (int)nLights, errCode);

    // Setup text shader
    auto textShader = Resources::GetShader("Text");
    textShader->compile(errCode);
    const glm::mat4 orthoProjection = glm::ortho(0.0f, (GLfloat)window->getWidth(), 0.0f,
                                                 (GLfloat)window->getHeight(), 0.0f, 1.0f);
    textShader->setMat4("projection", orthoProjection, errCode);

    // Setup light icon particle shader
    auto liShader = Resources::GetShader("LightIconParticle");
    liShader->compile(errCode);
    liShader->setMat4("projection", orthoProjection, errCode);

    // Setup camera
    Camera::init(glm::vec3(0.0f), {0.0f, 1.0f, 0.0f}, 90.0f, 0.0f, FOV, window->getAspectRatio(),
                 NEAR_PLANE, FAR_PLANE);
    auto camera = Camera::getInstance();

    // Main program loop
    while (!window->getShouldClose()) {
      // Get time elapsed since last cycle
      GLfloat timeNow = static_cast<GLfloat>(glfwGetTime());
      deltaTime = timeNow - lastTime;
      lastTime = timeNow;
      if (fpsBufferIdx < FPS_BUFFER_SIZE)
        fpsBuffer[fpsBufferIdx++] = 1.0f / deltaTime;

      // Handle user input events
      glfwPollEvents();
      camera->keyControl(window->getKeys(), deltaTime);
      camera->mouseControl(window->getDeltaX(), window->getDeltaY(), deltaTime);

      if (window->getToggleKey(GLFW_KEY_M, NULL))
        displayHUD = !displayHUD;

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Apply lights to lighting shader
      lightingShader->use();
      for (size_t i = 0; i < nLights; i++)
        sceneLights[i]->use(*lightingShader, i, errCode);

      // Add camera parameters to lighting shader
      lightingShader->setMat4("view", camera->getView(), errCode);
      lightingShader->setMat4("projection", camera->getProjection(), errCode);
      lightingShader->setVec3("viewPos", camera->getPosition(), errCode);

      // Update and draw each model
      for (const auto &model : modelList) {
        model->update(errCode, deltaTime);
        model->draw(*lightingShader, errCode);

        if (errCode != ERROR_OK)
          throw std::runtime_error("An error occurred while processing model " +
                                   std::to_string(model->_id) + ".");
      }

      // Draw light icons
      liShader->use();
      liShader->setMat4("view", camera->getView(), errCode);
      liShader->setMat4("projection", camera->getProjection(), errCode);
      for (const auto &icon : lightIcons)
        icon->draw(*liShader, errCode);

      // Draw HUD elements to screen
      if (displayHUD) {
        if (fpsUpdateTime >= FPS_UPDATE_DELAY) {
          // Calculate average fps from buffer
          GLfloat avgFps =
              std::accumulate(fpsBuffer.begin(), fpsBuffer.begin() + fpsBufferIdx, 0.0f) /
              std::max(fpsBufferIdx, (size_t)1);
          fpsValueText->setText(toStringDp(avgFps, 1));
          fpsUpdateTime -= FPS_UPDATE_DELAY;
          fpsBufferIdx = 0;
        } else {
          fpsUpdateTime += deltaTime;
        }

        const auto &cameraPos = camera->getPosition();

        xPosValueText->setText(toStringDp(cameraPos.x, 3));
        yPosValueText->setText(toStringDp(cameraPos.y, 3));
        zPosValueText->setText(toStringDp(cameraPos.z, 3));
        pitchValueText->setText(toStringDp(camera->getPitch(), 1));
        yawValueText->setText(toStringDp(camera->getYaw(), 1));

        textShader->use();
        for (auto it = textObjects.begin(); it != textObjects.end() && errCode == ERROR_OK; ++it)
          (*it)->draw(*textShader, errCode);

        if (errCode != ERROR_OK)
          throw std::runtime_error("An error occurred while drawing HUD element.");
      }

      window->swapBuffers();
    }
  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}