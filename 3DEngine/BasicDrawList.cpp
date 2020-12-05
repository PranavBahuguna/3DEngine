#include "BasicDrawList.h"

#include <algorithm>

ShaderSptr BasicDrawList::getShader() {
  m_shader->use();
  return m_shader;
}

DrawTargets &BasicDrawList::getTargets() { return m_drawTargets; }

void BasicDrawList::addTarget(const std::shared_ptr<GameObject> &target) {
  m_drawTargets.emplace_back(target);
}

void BasicDrawList::addTargets(const DrawTargets &targets) {
  std::move(targets.begin(), targets.end(), std::back_inserter(m_drawTargets));
}

void BasicDrawList::setShader(const ShaderSptr &shader) { m_shader = shader; }

// Basic implementation of draw switches to the stored shader program and calls the draw function of
// each target
void BasicDrawList::draw(ERROR &errCode) {
  ShaderSptr shader = getShader();
  const auto &targets = getTargets();

  for (size_t i = 0; i < targets.size() && errCode == ERROR_OK; ++i)
    targets[i]->draw(errCode, *shader);
}
