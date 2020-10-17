#pragma once

#include "DrawList.h"

class BasicDrawList : public DrawList {
public:
  ShaderSptr getShader() override;
  DrawTargets &getTargets() override;

  void draw(ERROR &errCode) override;
  void addTarget(const DrawSptr &target) override;
  void addTargets(const DrawTargets &targets) override;
  void setShader(ShaderSptr shader) override;

private:
  ShaderSptr m_shader;
  DrawTargets m_drawTargets;
};
