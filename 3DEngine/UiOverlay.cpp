#include "UiOverlay.h"
#include "DrawListBuilder.h"
#include "Game.h"
#include "Timer.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

// Converts a float to string with a number of decimal places
static std::string toStringDp(float f, size_t dp) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(dp) << f;
  return ss.str();
}

UiOverlay::UiOverlay() : m_toggleOn(true) {
  // Setup shader and draw list
  auto textShader = ResourceManager<Shader>::Get("Text");
  m_dlText = DrawListBuilder::CreateDrawList(textShader);
  m_dlText = DrawListBuilder::AddOrthoProjection(std::move(m_dlText));

  const Window &window = Game::GetWindow();

  m_fpsLabel = std::shared_ptr<GameObject>(new GameObject());
  m_fpsLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.95f}));
  m_fpsLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_SEAWEED, "FPS:");

  m_fpsValue = std::shared_ptr<GameObject>(new GameObject());
  m_fpsValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.95f}));
  m_fpsValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_SEAWEED);

  m_xPosLabel = std::shared_ptr<GameObject>(new GameObject());
  m_xPosLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.85f}));
  m_xPosLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_RED, "X:");

  m_xPosValue = std::shared_ptr<GameObject>(new GameObject());
  m_xPosValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.85f}));
  m_xPosValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_RED);

  m_yPosLabel = std::shared_ptr<GameObject>(new GameObject());
  m_yPosLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.8f}));
  m_yPosLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_GREEN, "Y:");

  m_yPosValue = std::shared_ptr<GameObject>(new GameObject());
  m_yPosValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.8f}));
  m_yPosValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_GREEN);

  m_zPosLabel = std::shared_ptr<GameObject>(new GameObject());
  m_zPosLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.75f}));
  m_zPosLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_BLUE, "Z:");

  m_zPosValue = std::shared_ptr<GameObject>(new GameObject());
  m_zPosValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.75f}));
  m_zPosValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_BLUE);

  m_pitchLabel = std::shared_ptr<GameObject>(new GameObject());
  m_pitchLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.65f}));
  m_pitchLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_YELLOW, "Pitch:");

  m_pitchValue = std::shared_ptr<GameObject>(new GameObject());
  m_pitchValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.65f}));
  m_pitchValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_YELLOW);

  m_yawLabel = std::shared_ptr<GameObject>(new GameObject());
  m_yawLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.6f}));
  m_yawLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_VIOLET, "Yaw:");

  m_yawValue = std::shared_ptr<GameObject>(new GameObject());
  m_yawValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.6f}));
  m_yawValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_VIOLET);

  m_fovLabel = std::shared_ptr<GameObject>(new GameObject());
  m_fovLabel->AddComponent<Transform>(window.relToWinPos({0.7f, 0.5f}));
  m_fovLabel->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_GREY, "FOV:");

  m_fovValue = std::shared_ptr<GameObject>(new GameObject());
  m_fovValue->AddComponent<Transform>(window.relToWinPos({0.8f, 0.5f}));
  m_fovValue->AddComponent<Text>(HUD_FONT, 1.0f, COLOR_GREY);

  m_dlText->addTargets({m_fpsLabel, m_fpsValue, m_xPosLabel, m_xPosValue, m_yPosLabel, m_yPosValue,
                        m_zPosLabel, m_zPosValue, m_pitchLabel, m_pitchValue, m_yawLabel,
                        m_yawValue, m_fovLabel, m_fovValue});

  // Initially, toggle all elements off
  toggle();
}

// Start toggling process to toggle UI elements on / off
void UiOverlay::toggle() {
  m_toggleOn = !m_toggleOn;

  for (auto &text : m_dlText->getTargets()) {
    text->setActive(m_toggleOn);
    text->setVisible(m_toggleOn);
  }
}

// Update the UI
void UiOverlay::update(ERROR &errCode) {
  static std::vector<float> fpsBuffer(FPS_BUFFER_SIZE);
  static size_t fpsBufferIdx = 0;
  static float fpsUpdateTime = FPS_UPDATE_DELAY;

  if (fpsBufferIdx < FPS_BUFFER_SIZE)
    fpsBuffer[fpsBufferIdx++] = 1.0f / Timer::GetDeltaTime();

  if (fpsUpdateTime >= FPS_UPDATE_DELAY) {
    // Calculate average fps from buffer
    float avgFps = std::accumulate(fpsBuffer.begin(), fpsBuffer.begin() + fpsBufferIdx, 0.0f) /
                   std::max(fpsBufferIdx, (size_t)1);
    m_fpsValue->GetComponent<Text>()->setText(toStringDp(avgFps, 1));
    fpsUpdateTime -= FPS_UPDATE_DELAY;
    fpsBufferIdx = 0;
  } else {
    fpsUpdateTime += Timer::GetDeltaTime();
  }

  const auto &cameraTransform = Game::GetCamera().transform();
  const auto &cameraProjection = Game::GetCamera().projection();

  m_xPosValue->GetComponent<Text>()->setText(toStringDp(cameraTransform.getPosition().x, 3));
  m_yPosValue->GetComponent<Text>()->setText(toStringDp(cameraTransform.getPosition().y, 3));
  m_zPosValue->GetComponent<Text>()->setText(toStringDp(cameraTransform.getPosition().z, 3));
  m_pitchValue->GetComponent<Text>()->setText(
      toStringDp(RAD2DEG(cameraTransform.getRotation().x), 1));
  m_yawValue->GetComponent<Text>()->setText(
      toStringDp(RAD2DEG(cameraTransform.getRotation().y), 1));
  m_fovValue->GetComponent<Text>()->setText(toStringDp(RAD2DEG(cameraProjection.getFOV()), 1));

  m_dlText->draw(errCode);
}