#pragma once

#include "DrawList.h"
#include "Light.h"

class IlluminationDrawList : public DrawListDecorator {
public:
  IlluminationDrawList(DrawListUptr drawList);

  void draw(ERROR &errCode) override;

  void setLights(const std::vector<std::string> &lightNames);

private:
  std::vector<LightSptr> m_lights;
};