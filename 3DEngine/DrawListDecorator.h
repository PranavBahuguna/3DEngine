#pragma once

#include "DrawList.h"

class DrawListDecorator : public DrawList {
public:
  DrawListDecorator(DrawListUptr drawList);

  virtual ShaderSptr getShader() override;
  virtual DrawTargets &getTargets() override;

  void draw(ERROR &errCode) override;
  void addTarget(const std::shared_ptr<GameObject> &target) override;
  void addTargets(const DrawTargets &targets) override;
  void setShader(const ShaderSptr &shader) override;

private:
  DrawListUptr m_drawList;
};
