#include "IlluminationDrawList.h"
#include "Game.h"

#include <stdexcept>

IlluminationDrawList::IlluminationDrawList(DrawListUptr drawList,
                                           const std::vector<LightSptr> &lights)
    : DrawListDecorator(std::move(drawList)), m_lights(lights) {}

void IlluminationDrawList::draw(ERROR &errCode) {
  ShaderSptr shader = getShader();

  // Apply all lights to the shader
  for (size_t i = 0; i < m_lights.size(); i++) {
    m_lights[i]->use(*shader, i);

    // Apply light space matrix if this is a shadow-casting light
    if (m_lights[i]->isShadowCaster())
      m_lights[i]->setLightSpaceMatrix(*shader);
  }

  const auto &cameraTransform = Game::GetCamera().transform();
  const auto &cameraProjection = Game::GetCamera().projection();

  shader->setMat4("projection", cameraProjection.getProjection());
  shader->setMat4("view", cameraTransform.getView());
  shader->setVec3("viewPos", cameraTransform.getPosition());

  DrawListDecorator::draw(errCode);
}