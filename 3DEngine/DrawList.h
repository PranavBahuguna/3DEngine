#pragma once

#include "Camera.h"
#include "Drawable.h"
#include "Resources.h"

class DrawList;

using DrawSptr = std::shared_ptr<Drawable>;
using DrawTargets = std::vector<DrawSptr>;
using DrawListUptr = std::unique_ptr<DrawList>;

class DrawList {
public:
  virtual void draw(ERROR &errCode) = 0;

  void setShader(const std::string &name);
  void setTargets(DrawTargets targets);

  virtual ShaderSptr getShader() const { return m_shader; }
  virtual DrawTargets &getTargets() { return m_drawTargets; }

private:
  ShaderSptr m_shader;
  DrawTargets m_drawTargets;
};

class BasicDrawList : public DrawList {
public:
  void draw(ERROR &errCode) override;
};

class DrawListDecorator : public DrawList {
public:
  DrawListDecorator(DrawListUptr drawList);

  void draw(ERROR &errCode) override { _drawList->draw(errCode); }

  ShaderSptr getShader() const override { return _drawList->getShader(); }
  DrawTargets &getTargets() override { return _drawList->getTargets(); }

protected:
  DrawListUptr _drawList;
};
