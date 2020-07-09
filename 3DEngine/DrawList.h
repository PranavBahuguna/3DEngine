#pragma once

#include "Camera.h"
#include "Drawable.h"
#include "Resources.h"

class DrawList;

using DrawPtr = std::shared_ptr<Drawable>;
using DrawTargets = std::vector<DrawPtr>;
using DListPtr = std::unique_ptr<DrawList>;

class DrawList {
public:
  virtual void draw(ERROR &errCode) = 0;

  void setShader(const std::string &name);
  void setTargets(DrawTargets targets);

  virtual ShaderPtr getShader() const { return m_shader; }
  virtual DrawTargets &getTargets() { return m_drawTargets; }

private:
  ShaderPtr m_shader;
  DrawTargets m_drawTargets;
};

class BasicDrawList : public DrawList {
public:
  void draw(ERROR &errCode) override;
};

class DrawListDecorator : public DrawList {
public:
  DrawListDecorator(DListPtr drawList);

  void draw(ERROR &errCode) override { _drawList->draw(errCode); }

  ShaderPtr getShader() const override { return _drawList->getShader(); }
  DrawTargets &getTargets() override { return _drawList->getTargets(); }

protected:
  DListPtr _drawList;
};
