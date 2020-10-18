#define TINYOBJLOADER_IMPLEMENTATION

#include "DrawListBuilder.h"
#include "Drawable.h"
#include "FrameBuffer.h"
#include "Game.h"
#include "GameObject.h"
#include "Keyboard.h"
#include "Light.h"
#include "LightIcon.h"
#include "Mouse.h"
#include "Plane.h"
#include "RenderBuffer.h"
#include "Skybox.h"
#include "Text.h"
#include "Timer.h"

#include <algorithm>
#include <iomanip>
#include <numeric>

using LiSptr = std::shared_ptr<LightIcon>;
using GObjSptr = std::shared_ptr<GameObject>;
using MeshSptr = std::shared_ptr<Mesh>;

ERROR errCode = ERROR_OK;

bool updateScene = true;

std::vector<ModelSptr> models;
std::vector<GObjSptr> gameObjects;
std::vector<LightSptr> lights;

TexSptr depthMap;
MeshSptr depthMesh;
constexpr GLuint SHADOW_WIDTH = 4096;
constexpr GLuint SHADOW_HEIGHT = 4096;

DrawListUptr dlIllum;
DrawListUptr dlDepth;
DrawListUptr dlShadowMapped;
DrawListUptr dlTrans;
DrawListUptr dlText;
DrawListUptr dlSkybox;

int main() {
  try {
    // Setup game
    Game::Init();
    Camera &camera = Game::GetCamera();
    Window &window = Game::GetWindow();

    // Setup shaders
    ShaderSptr lightingShader = ResourceManager<Shader>::Create("Lighting");
    lightingShader->setPreprocessor(GL_FRAGMENT_SHADER, "MAX_LIGHTS", MAX_LIGHTS);
    lightingShader->compile();

    ShaderSptr depthShader = ResourceManager<Shader>::Create("Depth");
    depthShader->compile();
    depthShader->setInt("depthMap", 0);
    depthShader->setFloat("nearPlane", 1.0f);
    depthShader->setFloat("farPlane", 20.0f);
    depthShader->setBool("isPerspective", true);

    ShaderSptr smdShader = ResourceManager<Shader>::Create("ShadowMappingDepth");
    smdShader->compile();

    ShaderSptr liParticleShader = ResourceManager<Shader>::Create("LightIconParticle");
    liParticleShader->compile();

    ShaderSptr textShader = ResourceManager<Shader>::Create("Text");
    textShader->compile();

    ShaderSptr skyboxShader = ResourceManager<Shader>::Create("Skybox");
    skyboxShader->compile();

    // Setup scene lights
    LightSptr light01(new DirectionalLight({1.0f, 1.0f, -1.0f}, glm::vec3(1.0f), 0.1f, 0.5f, 0.5f));
    LightSptr light02(new PointLight({4.0f, 4.0f, -4.0f}, {1.0f, 0.0f, 0.0f}, 0.1f, 1.0f, 1.0f,
                                     1.0f, 0.045f, 0.0075f));
    LightSptr light03(new SpotLight({-4.0f, 10.0f, 3.0f}, glm::vec3(1.0f), 0.1f, 1.0f, 1.0f, 1.0f,
                                    0.045f, 0.0075f, {0.0f, -0.99f, 0.0f}, 30.0f, 35.0f, true));

    lights = {light01, light02, light03};

    // Setup drawlists
    dlIllum = DrawListBuilder::CreateDrawList(lightingShader);
    dlIllum = DrawListBuilder::AddIllumination(std::move(dlIllum), lights);

    dlShadowMapped = DrawListBuilder::CreateDrawList(smdShader);
    dlShadowMapped = DrawListBuilder::AddShadowMapping(std::move(dlShadowMapped), lights);

    dlTrans = DrawListBuilder::CreateDrawList(liParticleShader);
    dlTrans = DrawListBuilder::AddTransparency(std::move(dlTrans));

    dlText = DrawListBuilder::CreateDrawList(textShader);
    dlText = DrawListBuilder::AddOrthoProjection(std::move(dlText));

    dlSkybox = DrawListBuilder::CreateDrawList(skyboxShader);

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

    // Setup scene objects
    ModelSptr tetrahedron(new Model("Tetrahedron"));
    ModelSptr cube(new Model("Cube"));
    ModelSptr earth(new Model("Sphere"));
    ModelSptr starfighter(new Model("Arc170"));
    ModelSptr floor(new Plane("Grass", {5, 5}, {10.0f, 10.0f}));
    floor->setPos(glm::vec3(0.0f, -3.0f, 0.0f));
    floor->setEuler(glm::vec3(1.0f));
    floor->load(errCode);

    dlIllum->addTargets({tetrahedron, cube, earth, starfighter, floor});
    dlShadowMapped->addTargets({tetrahedron, cube, earth, starfighter});

    GObjSptr earthObj(new GameObject(earth, "Earth"));
    GObjSptr tetrahedronObj(new GameObject(tetrahedron, "Tetrahedron"));
    GObjSptr cubeObj(new GameObject(cube, "Cube"));
    GObjSptr starfighterObj(new GameObject(starfighter, "Starfighter"));
    GObjSptr floorObj(new GameObject(floor));

    gameObjects = {earthObj, tetrahedronObj, cubeObj, starfighterObj, floorObj};

    // Setup light icons
    LiSptr li01(new LightIcon(light01));
    LiSptr li02(new LightIcon(light02));
    LiSptr li03(new LightIcon(light03));

    dlTrans->addTargets({li01, li02, li03});

    // Setup skybox
    Skybox skybox("Teide",
                  {"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"});

    dlSkybox->addTarget(std::make_shared<Skybox>(skybox));

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

    // Main program loop
    while (!window.getShouldClose()) {
      Timer::Update();

      // Handle user input events
      glfwPollEvents();
      Keyboard::KeyControl();
      Mouse::MouseControl();
      Mouse::MouseScrollControl();
      camera.update();

      // if (Window::GetToggleKey(errCode, GLFW_KEY_ENTER))
      //  updateScene = !updateScene;*/

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (updateScene) {
        // Update all gameobjects and lights
        for (const auto &gameObject : gameObjects)
          gameObject->update(errCode);

        for (const auto &light : lights)
          light->update(errCode);
      }

      // Render scene depth to texture from light's perspective
      glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
      depthMapFBO.bind();
      glClear(GL_DEPTH_BUFFER_BIT);
      dlShadowMapped->draw(errCode);
      depthMapFBO.unbind();

      // Reset viewport
      glViewport(0, 0, window.getWidth(), window.getHeight());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw all models first
      dlIllum->draw(errCode);

      if (USE_DEPTH_VISUALISATION) {
        depthShader->use();
        depthMap->use();
        depthMesh->draw();
      }

      // Draw the skybox
      dlSkybox->draw(errCode);

      // Draw all transparent objects next
      dlTrans->draw(errCode);

      // Update the HUD overlay
      Game::GetUiOverlay().update(errCode);

      if (errCode != ERROR_OK)
        throw std::runtime_error("An error occurred while runnning this level.");

      window.swapBuffers();
    }

  } catch (std::exception &e) {
    printf(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
