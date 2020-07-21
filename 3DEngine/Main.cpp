#define TINYOBJLOADER_IMPLEMENTATION

#include "Camera.h"
#include "DrawListBuilder.h"
#include "Drawable.h"
#include "GameObject.h"
#include "Light.h"
#include "LightIcon.h"
#include "Terrain.h"
#include "Text.h"
#include "Timer.h"
#include "Window.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

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
#define COLOR_GREY glm::vec4(0.6667f, 0.6627f, 0.6784f, 1.0f)
#define FPS_UPDATE_DELAY 0.5f
#define FPS_BUFFER_SIZE 8

using TextPtr = std::shared_ptr<Text>;
using LiPtr = std::shared_ptr<LightIcon>;

ERROR errCode = ERROR_OK;

std::vector<GLfloat> fpsBuffer(FPS_BUFFER_SIZE);
size_t fpsBufferIdx = 0;
GLfloat fpsUpdateTime = FPS_UPDATE_DELAY;

bool displayHUD = false;

std::vector<ModelPtr> models;

DrawTargets dtModels;
DrawTargets dtTexts;
DrawTargets dtLightIcons;

DListPtr dl_illum;
DListPtr dl_trans;
DListPtr dl_text;

std::vector<LightPtr> sceneLights;
size_t nLights = 0;

// Converts a float to string with a number of decimal places
static std::string toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}

int main() {
  try {
    // Determine the window mode to use and create a window
    WindowMode wMode = (!USE_WINDOWED) ? WindowMode::FULLSCREEN
                                       : (FULLSCREEN_WINDOWS) ? WindowMode::FULLSCREEN_WINDOWED
                                                              : WindowMode::WINDOWED;

    Window::Init("Test window", wMode, WINDOW_WIDTH, WINDOW_HEIGHT, errCode);

    // Allow objects to obscure other objects behind them
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup scene objects
    ModelPtr tetrahedron(new Model("Tetrahedron"));
    tetrahedron->setPos(glm::vec3(0.0f, 0.0f, 3.0f));
    tetrahedron->setScale(glm::vec3(0.4f));
    tetrahedron->load(errCode);

    ModelPtr cube(new Model("Cube"));
    cube->setPos(glm::vec3(-3.0f, 0.0f, 6.0f));
    cube->setScale(glm::vec3(0.4f));
    cube->load(errCode);

    ModelPtr earth(new Model("Sphere"));
    earth->setPos(glm::vec3(3.0f, 0.0f, 6.0f));
    earth->setEuler(glm::vec3(0.0f, 1.0f, 0.0f));
    earth->setScale(glm::vec3(0.4f));
    earth->load(errCode);

    ModelPtr starfighter(new Model("Arc170"));
    starfighter->setPos(glm::vec3(0.0f, 0.0f, 10.0f));
    starfighter->setEuler(glm::vec3(0.0f, 1.0f, 0.0f));
    starfighter->setAngle(180.0f);
    starfighter->setScale(glm::vec3(0.002f));
    starfighter->load(errCode);

    ModelPtr floor(new Terrain("Grass", {5, 5}, {5.0f, 5.0f}));
    floor->setPos(glm::vec3(0.0f, -3.0f, 0.0f));
    floor->setEuler(glm::vec3(1.0f));
    floor->load(errCode);

    GameObject earthObj("Earth");
    earthObj.setModel(earth);
    earthObj.loadScript("Earth", errCode);

    models = {tetrahedron, cube, earth, starfighter, floor};

    // Setup HUD elements
    TextPtr fpsLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.95f}), 1.0f, COLOR_SEAWEED, "FPS:"));
    TextPtr fpsValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.95f}), 1.0f, COLOR_SEAWEED));

    TextPtr xPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.85f}), 1.0f, COLOR_RED, "X:"));
    TextPtr xPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.85f}), 1.0f, COLOR_RED));

    TextPtr yPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.8f}), 1.0f, COLOR_GREEN, "Y:"));
    TextPtr yPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.8f}), 1.0f, COLOR_GREEN));

    TextPtr zPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.75f}), 1.0f, COLOR_BLUE, "Z:"));
    TextPtr zPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.75f}), 1.0f, COLOR_BLUE));

    TextPtr pitchLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.65f}), 1.0f, COLOR_YELLOW, "Pitch:"));
    TextPtr pitchValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.65f}), 1.0f, COLOR_YELLOW));

    TextPtr yawLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.6f}), 1.0f, COLOR_VIOLET, "Yaw:"));
    TextPtr yawValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.6f}), 1.0f, COLOR_VIOLET));

    TextPtr fovLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.5f}), 1.0f, COLOR_GREY, "FOV:"));
    TextPtr fovValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.5f}), 1.0f, COLOR_GREY));

    dtTexts = {fpsLabel,  fpsValue,   xPosLabel,  xPosValue, yPosLabel, yPosValue, zPosLabel,
               zPosValue, pitchLabel, pitchValue, yawLabel,  yawValue,  fovLabel,  fovValue};

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
    nLights = std::min(sceneLights.size(), (size_t)8);

    // Setup light icons
    LiPtr li01(new LightIcon("DirectionalLight"));
    LiPtr li02(new LightIcon("PointLight"));
    LiPtr li03(new LightIcon("SpotLight"));

    dtLightIcons = {li01, li02, li03};

    // Setup text shader
    auto textShader = Resources::GetShader("Text");
    textShader->compile(errCode);
    const glm::mat4 orthoProjection =
        glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight(), 0.0f, 1.0f);
    textShader->setMat4("projection", orthoProjection, errCode);

    // Setup drawlists
    dtModels = DrawTargets(models.begin(), models.end());
    dl_illum = DrawListBuilder::CreateDrawList(dtModels, "Lighting");
    dl_illum = DrawListBuilder::AddIllumination(std::move(dl_illum),
                                                {"DirectionalLight", "PointLight", "SpotLight"});

    dl_trans = DrawListBuilder::CreateDrawList(dtLightIcons, "LightIconParticle");
    dl_trans = DrawListBuilder::AddTransparency(std::move(dl_trans));

    dl_text = DrawListBuilder::CreateDrawList(dtTexts, "Text");

    // Setup camera
    Camera::Init(glm::vec3(0.0f), {0.0f, 1.0f, 0.0f}, 90.0f, 0.0f, FOV, NEAR_PLANE, FAR_PLANE);

    // Main program loop
    while (!Window::GetShouldClose()) {
      // Update the timer
      Timer::Update();

      if (fpsBufferIdx < FPS_BUFFER_SIZE)
        fpsBuffer[fpsBufferIdx++] = 1.0f / Timer::GetDeltaTime();

      // Handle user input events
      glfwPollEvents();
      Camera::KeyControl();
      Camera::MouseControl();
      Camera::MouseScrollControl();

      if (Window::GetToggleKey(GLFW_KEY_M, NULL))
        displayHUD = !displayHUD;

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update all models
      for (const auto &model : models)
        model->update(errCode);

      earthObj.update(errCode);

      // Draw all models and light icons
      dl_illum->draw(errCode);
      dl_trans->draw(errCode);

      // Draw HUD elements to screen
      if (displayHUD) {
        if (fpsUpdateTime >= FPS_UPDATE_DELAY) {
          // Calculate average fps from buffer
          GLfloat avgFps =
              std::accumulate(fpsBuffer.begin(), fpsBuffer.begin() + fpsBufferIdx, 0.0f) /
              std::max(fpsBufferIdx, (size_t)1);
          fpsValue->setText(toStringDp(avgFps, 1));
          fpsUpdateTime -= FPS_UPDATE_DELAY;
          fpsBufferIdx = 0;
        } else {
          fpsUpdateTime += Timer::GetDeltaTime();
        }

        xPosValue->setText(toStringDp(Camera::GetPosition().x, 3));
        yPosValue->setText(toStringDp(Camera::GetPosition().y, 3));
        zPosValue->setText(toStringDp(Camera::GetPosition().z, 3));
        pitchValue->setText(toStringDp(Camera::GetPitch(), 1));
        yawValue->setText(toStringDp(Camera::GetYaw(), 1));
        fovValue->setText(toStringDp(Camera::GetFOV(), 1));

        dl_text->draw(errCode);
      }

      Window::SwapBuffers();
    }
  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}