#pragma once

#include "DrawList.h"
#include "Text.h"

class UiOverlay {
public:
  UiOverlay();

  void toggle();
  void update(ERROR &errCode);

private:
  DrawListUptr m_dlText;
  bool m_toggleOn;

  // UI text elements
  std::shared_ptr<GameObject> m_fpsLabel;
  std::shared_ptr<GameObject> m_fpsValue;
  std::shared_ptr<GameObject> m_xPosLabel;
  std::shared_ptr<GameObject> m_xPosValue;
  std::shared_ptr<GameObject> m_yPosLabel;
  std::shared_ptr<GameObject> m_yPosValue;
  std::shared_ptr<GameObject> m_zPosLabel;
  std::shared_ptr<GameObject> m_zPosValue;
  std::shared_ptr<GameObject> m_pitchLabel;
  std::shared_ptr<GameObject> m_pitchValue;
  std::shared_ptr<GameObject> m_yawLabel;
  std::shared_ptr<GameObject> m_yawValue;
  std::shared_ptr<GameObject> m_fovLabel;
  std::shared_ptr<GameObject> m_fovValue;
};