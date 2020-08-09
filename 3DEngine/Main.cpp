#define TINYOBJLOADER_IMPLEMENTATION

#include "Camera.h"
#include "DrawListBuilder.h"
#include "Drawable.h"
#include "GameObject.h"
#include "Light.h"
#include "LightIcon.h"
#include "Plane.h"
#include "Text.h"
#include "Timer.h"
#include "Window.h"

#include <algorithm>
#include <iomanip>
#include <numeric>

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
#define MAX_LIGHTS 8

using TextPtr = std::shared_ptr<Text>;
using LiPtr = std::shared_ptr<LightIcon>;

ERROR errCode = ERROR_OK;

std::vector<float> fpsBuffer(FPS_BUFFER_SIZE);
size_t fpsBufferIdx = 0;
float fpsUpdateTime = FPS_UPDATE_DELAY;

bool displayHUD = false;

std::vector<ModelSptr> models;
std::vector<GameObject *> gameObjects;
std::vector<LightSptr> lights;

DrawTargets dtModels;
DrawTargets dtTexts;
DrawTargets dtLightIcons;

DrawListUptr dl_illum;
DrawListUptr dl_trans;
DrawListUptr dl_text;

// forward declarations
static std::string toStringDp(float, size_t);

int main() {
  try {
    // Determine the window mode to use and create a window
    WindowMode wMode = (!USE_WINDOWED) ? WindowMode::FULLSCREEN
                                       : (FULLSCREEN_WINDOWS) ? WindowMode::FULLSCREEN_WINDOWED
                                                              : WindowMode::WINDOWED;

    Window::Init("Test window", wMode, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Allow objects to obscure other objects behind them
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup scene objects
    ModelSptr tetrahedron(new Model("Tetrahedron"));
    ModelSptr cube(new Model("Cube"));
    ModelSptr earth(new Model("Sphere"));
    ModelSptr starfighter(new Model("Arc170"));

    ModelSptr floor(new Plane("Grass", {5, 5}, {10.0f, 10.0f}));
    floor->setPos(glm::vec3(0.0f, -3.0f, 0.0f));
    floor->setEuler(glm::vec3(1.0f));
    floor->load(errCode);

    GameObject *earthObj(new GameObject(earth, "Earth"));
    GameObject *tetrahedronObj(new GameObject(tetrahedron, "Tetrahedron"));
    GameObject *cubeObj(new GameObject(cube, "Cube"));
    GameObject *starfighterObj(new GameObject(starfighter, "Starfighter"));
    GameObject *floorObj(new GameObject(floor));

    gameObjects = {earthObj, tetrahedronObj, cubeObj, starfighterObj, floorObj};
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
    LightSptr light01 =
        Resources<Light>::Create("DirectionalLight", LightType::DIRECTIONAL_LIGHT,
                                 glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f), 0.25f, 1.0f, 1.0f);
    LightSptr light02 = Resources<Light>::Create(
        "PointLight", LightType::POINT_LIGHT, glm::vec3(4.0f, 4.0f, -4.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), 0.25f, 1.0f, 1.0f, 1.0f, 0.045f, 0.0075f);
    LightSptr light03 = Resources<Light>::Create(
        "SpotLight", LightType::SPOT_LIGHT, glm::vec3(-4.0f, 10.0f, 3.0f), glm::vec3(1.0f), 0.25f,
        1.0f, 1.0f, 1.0f, 0.045f, 0.0075f, glm::vec3(0.0f, -1.0f, 0.0f), 20.0f, 25.0f);

    lights = {light01, light02, light03};

    // Setup light icons
    LiPtr li01(new LightIcon("DirectionalLight"));
    LiPtr li02(new LightIcon("PointLight"));
    LiPtr li03(new LightIcon("SpotLight"));

    dtLightIcons = {li01, li02, li03};

    // Setup lighting shader
    auto lightingShader = Resources<Shader>::Get("Lighting", "Lighting");
    lightingShader->setPreprocessor(GL_FRAGMENT_SHADER, "MAX_LIGHTS", MAX_LIGHTS);

    // Setup drawlists
    dtModels = DrawTargets(models.begin(), models.end());
    dl_illum = DrawListBuilder::CreateDrawList(dtModels, "Lighting");
    dl_illum = DrawListBuilder::AddIllumination(std::move(dl_illum),
                                                {"DirectionalLight", "PointLight", "SpotLight"});

    dl_trans = DrawListBuilder::CreateDrawList(dtLightIcons, "LightIconParticle");
    dl_trans = DrawListBuilder::AddTransparency(std::move(dl_trans));

    dl_text = DrawListBuilder::CreateDrawList(dtTexts, "Text");
    dl_text = DrawListBuilder::AddOrthoProjection(std::move(dl_text));

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

      if (Window::GetToggleKey(errCode, GLFW_KEY_M))
        displayHUD = !displayHUD;

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update all gameobjects
      for (const auto &gameObject : gameObjects)
        gameObject->update(errCode);

      // Draw all models and light icons
      dl_illum->draw(errCode);
      dl_trans->draw(errCode);

      // Draw HUD elements to screen
      if (displayHUD) {
        if (fpsUpdateTime >= FPS_UPDATE_DELAY) {
          // Calculate average fps from buffer
          float avgFps =
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

      if (errCode != ERROR_OK)
        throw std::runtime_error("An error occurred while runnning this level.");

      Window::SwapBuffers();
    }

  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Converts a float to string with a number of decimal places
static std::string toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}