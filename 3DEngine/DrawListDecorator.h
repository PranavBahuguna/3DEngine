#pragma once

#include "DrawList.h"

class DrawListDecorator : public DrawList {
public:
  DrawListDecorator(DrawListUptr drawList);

  virtual ShaderSptr getShader() override;
  virtual DrawTargets &getTargets() override;

  void draw(ERROR &errCode) override;
  void addTarget(const DrawSptr &target) override;
  void addTargets(const DrawTargets &targets) override;
  void setShader(ShaderSptr shader) override;

private:
  DrawListUptr m_drawList;
};
