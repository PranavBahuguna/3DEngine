#include "ShadowMappingDrawList.h"

ShadowMappingDrawList::ShadowMappingDrawList(DrawListUptr drawList,
                                             const std::vector<LightSptr> &lights)
    : DrawListDecorator(std::move(drawList)), m_lights(lights) {}

void ShadowMappingDrawList::draw(ERROR &errCode) {
  ShaderSptr _shader = _drawList->getShader();
  _shader->use();

  // Apply the light space matrix for all shadow-casting lights
  for (auto &light : m_lights) {
    if (light->isShadowCaster())
      light->setLightSpaceMatrix(*_shader);
  }

  DrawListDecorator::draw(errCode);
}