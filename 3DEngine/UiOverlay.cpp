#include "UiOverlay.h"
#include "DrawListBuilder.h"
#include "Game.h"
#include "Timer.h"

#include <algorithm>
#include <numeric>

UiOverlay::UiOverlay() : m_toggleOn(true) {
  // Setup shader and draw list
  auto textShader = ResourceManager<Shader>::Get("Text");
  textShader->compile();

  m_dlText = DrawListBuilder::CreateDrawList(textShader);
  m_dlText = DrawListBuilder::AddOrthoProjection(std::move(m_dlText));

  const Window &window = Game::GetWindow();

  // Setup HUD elements
  m_fpsLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.95f}), 1.0f, COLOR_SEAWEED, "FPS:"));
  m_fpsValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.95f}), 1.0f, COLOR_SEAWEED));
  m_xPosLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.85f}), 1.0f, COLOR_RED, "X:"));
  m_xPosValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.85f}), 1.0f, COLOR_RED));
  m_yPosLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.8f}), 1.0f, COLOR_GREEN, "Y:"));
  m_yPosValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.8f}), 1.0f, COLOR_GREEN));
  m_zPosLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.75f}), 1.0f, COLOR_BLUE, "Z:"));
  m_zPosValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.75f}), 1.0f, COLOR_BLUE));
  m_pitchLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.65f}), 1.0f, COLOR_YELLOW, "Pitch:"));
  m_pitchValue =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.65f}), 1.0f, COLOR_YELLOW));
  m_yawLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.6f}), 1.0f, COLOR_VIOLET, "Yaw:"));
  m_yawValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.6f}), 1.0f, COLOR_VIOLET));
  m_fovLabel =
      TextSptr(new Text(HUD_FONT, window.relToWinPos({0.7f, 0.5f}), 1.0f, COLOR_GREY, "FOV:"));
  m_fovValue = TextSptr(new Text(HUD_FONT, window.relToWinPos({0.8f, 0.5f}), 1.0f, COLOR_GREY));

  m_dlText->addTargets({m_fpsLabel, m_fpsValue, m_xPosLabel, m_xPosValue, m_yPosLabel, m_yPosValue,
                        m_zPosLabel, m_zPosValue, m_pitchLabel, m_pitchValue, m_yawLabel,
                        m_yawValue, m_fovLabel, m_fovValue});

  // Initially, toggle all elements off
  toggle();
}

// Start toggling process to toggle UI elements on / off
void UiOverlay::toggle() {
  m_toggleOn = !m_toggleOn;

  for (auto &text : m_dlText->getTargets())
    text->setActive(m_toggleOn);
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
    m_fpsValue->setText(Game::toStringDp(avgFps, 1));
    fpsUpdateTime -= FPS_UPDATE_DELAY;
    fpsBufferIdx = 0;
  } else {
    fpsUpdateTime += Timer::GetDeltaTime();
  }

  const auto &camera = Game::GetCamera();

  m_xPosValue->setText(Game::toStringDp(camera.getPosition().x, 3));
  m_yPosValue->setText(Game::toStringDp(camera.getPosition().y, 3));
  m_zPosValue->setText(Game::toStringDp(camera.getPosition().z, 3));
  m_pitchValue->setText(Game::toStringDp(glm::degrees(camera.getPitch()), 1));
  m_yawValue->setText(Game::toStringDp(glm::degrees(camera.getYaw()), 1));
  m_fovValue->setText(Game::toStringDp(glm::degrees(camera.getFOV()), 1));

  m_dlText->draw(errCode);
}