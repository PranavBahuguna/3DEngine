#pragma once

#include "Drawable.h"
#include "Shader.h"

class DrawList;

using DrawSptr = std::shared_ptr<Drawable>;
using DrawTargets = std::vector<DrawSptr>;
using DrawListUptr = std::unique_ptr<DrawList>;

class DrawList {
public:
  virtual ShaderSptr getShader() = 0;
  virtual DrawTargets &getTargets() = 0;

  virtual void draw(ERROR &errCode) = 0;
  virtual void addTarget(const DrawSptr &target) = 0;
  virtual void addTargets(const DrawTargets &targets) = 0;
  virtual void setShader(ShaderSptr shader) = 0;
};