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
  TextSptr m_fpsLabel;
  TextSptr m_fpsValue;
  TextSptr m_xPosLabel;
  TextSptr m_xPosValue;
  TextSptr m_yPosLabel;
  TextSptr m_yPosValue;
  TextSptr m_zPosLabel;
  TextSptr m_zPosValue;
  TextSptr m_pitchLabel;
  TextSptr m_pitchValue;
  TextSptr m_yawLabel;
  TextSptr m_yawValue;
  TextSptr m_fovLabel;
  TextSptr m_fovValue;
};