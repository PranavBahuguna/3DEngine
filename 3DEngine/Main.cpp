#define TINYOBJLOADER_IMPLEMENTATION

#include "Camera.h"
#include "DrawListBuilder.h"
#include "Drawable.h"
#include "FrameBuffer.h"
#include "GameObject.h"
#include "Light.h"
#include "LightIcon.h"
#include "Plane.h"
#include "RenderBuffer.h"
#include "Skybox.h"
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
#define DEPTH_VISUALISATION false

using TextSptr = std::shared_ptr<Text>;
using LiSptr = std::shared_ptr<LightIcon>;
using GObjSptr = std::shared_ptr<GameObject>;
using MeshSptr = std::shared_ptr<Mesh>;

ERROR errCode = ERROR_OK;

std::vector<float> fpsBuffer(FPS_BUFFER_SIZE);
size_t fpsBufferIdx = 0;
float fpsUpdateTime = FPS_UPDATE_DELAY;

bool displayHUD = false;

std::vector<ModelSptr> models;
std::vector<GObjSptr> gameObjects;
std::vector<LightSptr> lights;

TexSptr depthMap;
MeshSptr depthMesh;
constexpr GLuint SHADOW_WIDTH = 1024;
constexpr GLuint SHADOW_HEIGHT = 1024;

DrawTargets dtModels;
DrawTargets dtTexts;
DrawTargets dtLightIcons;
DrawTargets dtSkybox;

DrawListUptr dlIllum;
DrawListUptr dlDepth;
DrawListUptr dlShadowMapped;
DrawListUptr dlTrans;
DrawListUptr dlText;
DrawListUptr dlSkybox;

unsigned int quadVAO = 0;
unsigned int quadVBO = 0;

// forward declarations
static std::string toStringDp(float, size_t);

int main() {
  try {
    // Determine the window mode to use and create a window
    WindowMode wMode = (!USE_WINDOWED)        ? WindowMode::FULLSCREEN
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

    GObjSptr earthObj(new GameObject(earth, "Earth"));
    GObjSptr tetrahedronObj(new GameObject(tetrahedron, "Tetrahedron"));
    GObjSptr cubeObj(new GameObject(cube, "Cube"));
    GObjSptr starfighterObj(new GameObject(starfighter, "Starfighter"));
    GObjSptr floorObj(new GameObject(floor));

    gameObjects = {earthObj, tetrahedronObj, cubeObj, starfighterObj, floorObj};
    models = {tetrahedron, cube, earth, starfighter, floor};

    // Setup HUD elements
    TextSptr fpsLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.95f}), 1.0f, COLOR_SEAWEED, "FPS:"));
    TextSptr fpsValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.95f}), 1.0f, COLOR_SEAWEED));

    TextSptr xPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.85f}), 1.0f, COLOR_RED, "X:"));
    TextSptr xPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.85f}), 1.0f, COLOR_RED));

    TextSptr yPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.8f}), 1.0f, COLOR_GREEN, "Y:"));
    TextSptr yPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.8f}), 1.0f, COLOR_GREEN));

    TextSptr zPosLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.75f}), 1.0f, COLOR_BLUE, "Z:"));
    TextSptr zPosValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.75f}), 1.0f, COLOR_BLUE));

    TextSptr pitchLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.65f}), 1.0f, COLOR_YELLOW, "Pitch:"));
    TextSptr pitchValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.65f}), 1.0f, COLOR_YELLOW));

    TextSptr yawLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.6f}), 1.0f, COLOR_VIOLET, "Yaw:"));
    TextSptr yawValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.6f}), 1.0f, COLOR_VIOLET));

    TextSptr fovLabel(
        new Text(HUD_FONT, Window::RelToWinPos({0.7f, 0.5f}), 1.0f, COLOR_GREY, "FOV:"));
    TextSptr fovValue(new Text(HUD_FONT, Window::RelToWinPos({0.8f, 0.5f}), 1.0f, COLOR_GREY));

    dtTexts = {fpsLabel,  fpsValue,   xPosLabel,  xPosValue, yPosLabel, yPosValue, zPosLabel,
               zPosValue, pitchLabel, pitchValue, yawLabel,  yawValue,  fovLabel,  fovValue};

    // Setup scene lights
    LightSptr light01(new DirectionalLight({1.0f, 1.0f, -1.0f}, glm::vec3(1.0f), 0.1f, 0.5f, 0.5f));
    LightSptr light02(new PointLight({4.0f, 4.0f, -4.0f}, {1.0f, 0.0f, 0.0f}, 0.1f, 1.0f, 1.0f,
                                     1.0f, 0.045f, 0.0075f));
    LightSptr light03(new SpotLight({-4.0f, 10.0f, 3.0f}, glm::vec3(1.0f), 0.1f, 1.0f, 1.0f, 1.0f,
                                    0.045f, 0.0075f, {0.0f, -1.0f, 0.0f}, 30.0f, 35.0f, true));

    lights = {light01, light02, light03};

    // Setup light icons
    LiSptr li01(new LightIcon(light01));
    LiSptr li02(new LightIcon(light02));
    LiSptr li03(new LightIcon(light03));

    dtLightIcons = {li01, li02, li03};

    // Setup skybox
    Skybox skybox("Teide",
                  {"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"});

    // Setup lighting and depth shader
    auto lightingShader = ResourceManager<Shader>::Create("Lighting");
    lightingShader->setPreprocessor(GL_FRAGMENT_SHADER, "MAX_LIGHTS", MAX_LIGHTS);

    auto depthShader = ResourceManager<Shader>::Create("Depth");
    depthShader->compile();
    depthShader->setInt("depthMap", 0);
    depthShader->setFloat("nearPlane", 1.0f);
    depthShader->setFloat("farPlane", 20.0f);
    depthShader->setBool("isPerspective", true);

    auto SMD_Shader = ResourceManager<Shader>::Create("ShadowMappingDepth");
    SMD_Shader->compile();

    // Setup drawlists
    dtModels = DrawTargets(models.begin(), models.end());
    dlIllum = DrawListBuilder::CreateDrawList(dtModels, "Lighting");
    dlIllum = DrawListBuilder::AddIllumination(std::move(dlIllum), lights);

    dlShadowMapped = DrawListBuilder::CreateDrawList(dtModels, "ShadowMappingDepth");

    dlTrans = DrawListBuilder::CreateDrawList(dtLightIcons, "LightIconParticle");
    dlTrans = DrawListBuilder::AddTransparency(std::move(dlTrans));

    dlText = DrawListBuilder::CreateDrawList(dtTexts, "Text");
    dlText = DrawListBuilder::AddOrthoProjection(std::move(dlText));

    dtSkybox = DrawTargets({std::make_shared<Skybox>(skybox)});
    dlSkybox = DrawListBuilder::CreateDrawList(dtSkybox, "Skybox");

    // Setup shadow map
    depthMap = ResourceManager<Texture>::Create("depth-map", SHADOW_WIDTH, SHADOW_HEIGHT,
                                                (void *)NULL, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthMap->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    depthMap->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    depthMap->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    depthMap->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // black border
    depthMap->setParameter(GL_TEXTURE_BORDER_COLOR, borderColor);

    // Setup shadow map debugger mesh
    std::vector<float> dmVerts = {
        -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    };
    std::vector<float> dmTexCoords = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    std::vector<GLuint> dmIndices = {0, 1, 2, 2, 1, 3};
    depthMesh = ResourceManager<Mesh>::Create("depth-mesh", dmVerts, dmTexCoords,
                                              std::vector<float>(), dmIndices);

    // Setup FBO and attach shadow map to it
    FrameBuffer depthMapFBO;
    depthMapFBO.bind();
    depthMapFBO.attachTexture(depthMap->getId(), FboAttachment::Depth);
    glDrawBuffer(GL_NONE); // not drawing to any color buffer
    glReadBuffer(GL_NONE);
    if (depthMapFBO.isOk()) {
      printf("Framebuffer OK!\n");
    } else {
      printf("Framebuffer not OK!\n");
      return -1;
    }

    depthMapFBO.unbind();

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

      // Render scene depth to texture from light's perspective
      glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
      depthMapFBO.bind();
      glClear(GL_DEPTH_BUFFER_BIT);
      dlShadowMapped->draw(errCode);
      depthMapFBO.unbind();

      // Reset viewport
      glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw all models first
      dlIllum->draw(errCode);

      if (DEPTH_VISUALISATION) {
        depthShader->use();
        depthMap->use();
        depthMesh->draw();
      }

      // Draw the skybox
      dlSkybox->draw(errCode);

      // Draw all transparent objects next
      dlTrans->draw(errCode);

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

        dlText->draw(errCode);
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
