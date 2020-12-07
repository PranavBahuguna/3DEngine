#pragma once

#include "DrawList.h"

class BasicDrawList : public DrawList {
public:
  ShaderSptr getShader() override;
  DrawTargets &getTargets() override;

  void draw(ERROR &errCode) override;
  void addTarget(const std::shared_ptr<GameObject> &target) override;
  void addTargets(const DrawTargets &targets) override;
  void setShader(const ShaderSptr &shader) override;

private:
  ShaderSptr m_shader;
  DrawTargets m_drawTargets;
};
