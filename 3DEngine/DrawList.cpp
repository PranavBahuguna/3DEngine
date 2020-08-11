#include "DrawList.h"

#include <stdexcept>

void DrawList::setShader(const std::string &name) {
  m_shader = ResourceManager<Shader>::Get(name);
  if (!m_shader->isCompiled())
    m_shader->compile();
}

void DrawList::setTargets(DrawTargets targets) { m_drawTargets = targets; }

// Basic implementation of draw uses shader program and calls the draw function of each target
void BasicDrawList::draw(ERROR &errCode) {
  ShaderSptr _shader = getShader();
  _shader->use();

  const auto &targets = getTargets();
  for (auto it = targets.begin(); it != targets.end() && errCode == ERROR_OK; ++it)
    (*it)->draw(errCode, *_shader);
}

DrawListDecorator::DrawListDecorator(DrawListUptr drawList) : _drawList(std::move(drawList)) {}
