#include "DrawList.h"

#include <stdexcept>

void DrawList::setShader(const std::string &name) {
  m_shader = Resources::GetShader(name);
  if (!m_shader->isCompiled()) {
    ERROR errCode = ERROR_OK;
    m_shader->compile(errCode);
  }
}

void DrawList::setTargets(DrawTargets targets) { m_drawTargets = targets; }

// Basic implementation of draw uses shader program and calls the draw function of each target
void BasicDrawList::draw(ERROR &errCode) {
  ShaderPtr _shader = getShader();
  _shader->use();

  const auto &_targets = getTargets();
  auto targetIt = _targets.begin();
  for (; targetIt != _targets.end() && errCode == ERROR_OK; ++targetIt)
    (*targetIt)->draw(*_shader, errCode);

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while drawing target (" +
                             std::to_string((*targetIt)->_id) + ").");
}

DrawListDecorator::DrawListDecorator(DListPtr drawList) : _drawList(std::move(drawList)) {}
