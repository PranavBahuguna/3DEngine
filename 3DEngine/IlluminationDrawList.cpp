#include "IlluminationDrawList.h"
#include "Game.h"

#include <stdexcept>

IlluminationDrawList::IlluminationDrawList(DrawListUptr drawList,
                                           const std::vector<LightSptr> &lights)
    : DrawListDecorator(std::move(drawList)), m_lights(lights) {}

void IlluminationDrawList::draw(ERROR &errCode) {
  ShaderSptr shader = getShader();
  shader->use();

  // Apply all lights to the shader
  for (size_t i = 0; i < m_lights.size(); i++) {
    m_lights[i]->use(*shader, i);

    // Apply light space matrix if this is a shadow-casting light
    if (m_lights[i]->isShadowCaster())
      m_lights[i]->setLightSpaceMatrix(*shader);
  }

  shader->setMat4("projection", Game::GetCamera().getProjection());
  shader->setMat4("view", Game::GetCamera().getView());
  shader->setVec3("viewPos", Game::GetCamera().getPosition());

  DrawListDecorator::draw(errCode);
}