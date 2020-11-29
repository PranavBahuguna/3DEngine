#define TINYOBJLOADER_IMPLEMENTATION

#include "DrawListBuilder.h"
#include "FrameBuffer.h"
#include "Game.h"
#include "GameObject.h"
#include "Light.h"
#include "LightIcon.h"
#include "Plane.h"
#include "Script.h"
#include "Skybox.h"
#include "Timer.h"

#include <iomanip>

using LiSptr = std::shared_ptr<LightIcon>;
using GObjSptr = std::shared_ptr<GameObject>;
using MeshSptr = std::shared_ptr<Mesh>;

ERROR errCode = ERROR_OK;

std::vector<GObjSptr> gameObjects;
std::vector<LightSptr> lights;

TexSptr depthMap;
MeshSptr depthMesh;

DrawListUptr dlIllum;
DrawListUptr dlDepth;
DrawListUptr dlShadowMapped;
DrawListUptr dlTrans;
DrawListUptr dlSkybox;

int main() {
  try {
    // Setup game
    Game::Init();
    Camera &camera = Game::GetCamera();
    Window &window = Game::GetWindow();

    // Setup shaders
    PreprocessorList lightingShaderPreprocessors = {{GL_FRAGMENT_SHADER, "MAX_LIGHTS", MAX_LIGHTS}};
    auto lightingShader = ResourceManager<Shader>::Create("Lighting", lightingShaderPreprocessors);

    auto depthShader = ResourceManager<Shader>::Create("Depth");
    depthShader->use();
    depthShader->setInt("depthMap", 0);
    depthShader->setFloat("nearPlane", 1.0f);
    depthShader->setFloat("farPlane", 20.0f);
    depthShader->setBool("isPerspective", true);

    auto smdShader = ResourceManager<Shader>::Create("ShadowMappingDepth");
    auto liParticleShader = ResourceManager<Shader>::Create("LightIconParticle");
    auto skyboxShader = ResourceManager<Shader>::Create("Skybox");

    // Setup scene lights
    auto li01Rot = DEG2RAD(glm::vec3(-45.0f, -45.0f, 0.0f));
    auto li01Transform = Transform(glm::vec3(), li01Rot);
    auto li01Color = glm::vec3(1.0f);
    auto li01Phong = Phong{0.1f, 0.5f, 0.5f};
    LightSptr light01(new DirectionalLight(li01Transform, li01Color, li01Phong));

    auto li02Position = glm::vec3(4.0f, 4.0f, -4.0f);
    auto li02Color = glm::vec3(1.0f, 0.0f, 0.0f);
    auto li02Phong = Phong{0.1f, 1.0f, 1.0f};
    auto li02Att = Attenuation{1.0f, 0.045f, 0.0075f};
    LightSptr light02(new PointLight(li02Position, li02Color, li02Phong, li02Att));

    auto li03Position = glm::vec3(-4.0f, 10.0f, 3.0f);
    auto li03Rotation = DEG2RAD(glm::vec3(-90.0f, 0.0f, 0.0f));
    auto li03Transform = Transform(li03Position, li03Rotation);
    auto li03Color = glm::vec3(1.0f);
    auto li03Phong = Phong{0.1f, 1.0f, 1.0f};
    auto li03Att = Attenuation{1.0f, 0.045f, 0.0075f};
    auto li03InnerConeAngle = DEG2RAD(30.0f);
    auto li03OuterConeAngle = DEG2RAD(35.0f);
    auto li03ShadowCaster = true;
    LightSptr light03(new SpotLight(li03Transform, li03Color, li03Phong, li03Att,
                                    li03InnerConeAngle, li03OuterConeAngle, li03ShadowCaster));

    lights = {light01, light02, light03};

    // Setup drawlists
    dlIllum = DrawListBuilder::CreateDrawList(lightingShader);
    dlIllum = DrawListBuilder::AddIllumination(std::move(dlIllum), lights);

    dlShadowMapped = DrawListBuilder::CreateDrawList(smdShader);
    dlShadowMapped = DrawListBuilder::AddShadowMapping(std::move(dlShadowMapped), lights);

    dlTrans = DrawListBuilder::CreateDrawList(liParticleShader);
    dlTrans = DrawListBuilder::AddTransparency(std::move(dlTrans));

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
    GObjSptr tetrahedron(new GameObject());
    tetrahedron->AddComponent<Transform>();
    tetrahedron->AddComponent<Script>("Tetrahedron");
    auto tetrahedronModel = tetrahedron->AddComponent<Model>("Tetrahedron");
    tetrahedronModel->load(errCode);

    GObjSptr cube(new GameObject());
    cube->AddComponent<Transform>();
    cube->AddComponent<Script>("Cube");
    auto cubeModel = cube->AddComponent<Model>("Cube");
    cubeModel->load(errCode);

    GObjSptr earth(new GameObject());
    earth->AddComponent<Transform>();
    earth->AddComponent<Script>("Earth");
    auto earthModel = earth->AddComponent<Model>("Sphere");
    earthModel->load(errCode);

    GObjSptr starfighter(new GameObject());
    starfighter->AddComponent<Transform>();
    starfighter->AddComponent<Script>("Starfighter");
    auto starfighterModel = starfighter->AddComponent<Model>("Arc170");
    starfighterModel->load(errCode);

    GObjSptr floor(new GameObject());
    floor->AddComponent<Transform>(glm::vec3(0.0f, -3.0f, 0.0f));
    auto floorModel =
        floor->AddComponent<Plane>("Grass", glm::uvec2(5, 5), glm::vec2(10.0f, 10.0f));
    floorModel->load(errCode);

    dlIllum->addTargets({tetrahedronModel, cubeModel, earthModel, starfighterModel, floorModel});
    dlShadowMapped->addTargets({tetrahedronModel, cubeModel, earthModel, starfighterModel});
    gameObjects = {tetrahedron, cube, earth, starfighter, floor};

    // Setup light icons
    //LiSptr li01(new LightIcon(light01));
    //LiSptr li02(new LightIcon(light02));
    //LiSptr li03(new LightIcon(light03));

    //dlTrans->addTargets({li01, li02, li03});

    // Setup skybox
    GObjSptr skybox(new GameObject());
    skybox->AddComponent<Transform>();
    skybox->AddComponent<Skybox>("Teide",
                                 std::vector<std::string>({"posx.jpg", "negx.jpg", "posy.jpg",
                                                           "negy.jpg", "posz.jpg", "negz.jpg"}));

    dlSkybox->addTarget(skybox->GetComponent<Skybox>());

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

    for (const auto &gameObject : gameObjects)
      gameObject->init(errCode);

    // Main program loop
    while (!window.getShouldClose()) {
      Game::Update();

      // Update all gameobjects and lights
      if (Game::ShouldUpdateScene()) {
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
