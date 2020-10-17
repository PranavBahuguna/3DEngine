#include "BasicDrawList.h"

#include <algorithm>

ShaderSptr BasicDrawList::getShader() { return m_shader; }

DrawTargets &BasicDrawList::getTargets() { return m_drawTargets; }

void BasicDrawList::addTarget(const DrawSptr &target) { m_drawTargets.emplace_back(target); }

void BasicDrawList::addTargets(const DrawTargets &targets) {
  std::move(targets.begin(), targets.end(), std::back_inserter(m_drawTargets));
}

void BasicDrawList::setShader(ShaderSptr shader) { m_shader = shader; }

// Basic implementation of draw switches to the stored shader program and calls the draw function of
// each target
void BasicDrawList::draw(ERROR &errCode) {
  ShaderSptr shader = getShader();
  shader->use();

  const auto &targets = getTargets();
  for (auto it = targets.begin(); it != targets.end() && errCode == ERROR_OK; ++it)
    (*it)->draw(errCode, *shader);
}
