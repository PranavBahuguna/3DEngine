#pragma once

#include "DrawList.h"
#include "Light.h"

class ShadowMappingDrawList : public DrawListDecorator {
public:
  ShadowMappingDrawList(DrawListUptr drawList, const std::vector<LightSptr> &lights);

  void draw(ERROR &errCode) override;

private:
  std::vector<LightSptr> m_lights;
};