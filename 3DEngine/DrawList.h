#pragma once

#include "GameObject.h"
#include "Shader.h"

class DrawList;

using DrawTargets = std::vector<std::shared_ptr<GameObject>>;
using DrawListUptr = std::unique_ptr<DrawList>;

class DrawList {
public:
  virtual ShaderSptr getShader() = 0;
  virtual DrawTargets &getTargets() = 0;

  virtual void draw(ERROR &errCode) = 0;
  virtual void addTarget(const std::shared_ptr<GameObject> &target) = 0;
  virtual void addTargets(const DrawTargets &targets) = 0;
  virtual void setShader(const ShaderSptr &shader) = 0;
};