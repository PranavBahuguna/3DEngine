#pragma once

#include "DrawListDecorator.h"
#include "Light.h"

class IlluminationDrawList : public DrawListDecorator {
public:
  IlluminationDrawList(DrawListUptr drawList, const std::vector<LightSptr> &lights);

  void draw(ERROR &errCode) override;

private:
  std::vector<LightSptr> m_lights;
};