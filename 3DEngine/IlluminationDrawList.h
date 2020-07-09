#pragma once

#include "DrawList.h"

class IlluminationDrawList : public DrawListDecorator {
public:
  IlluminationDrawList(DListPtr drawList);
  ~IlluminationDrawList() = default;

  void draw(ERROR &errCode) override;

  void setLights(const std::vector<std::string> &lightNames);

private:
  std::vector<LightPtr> m_lights;
};