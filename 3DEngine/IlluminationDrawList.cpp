#include "IlluminationDrawList.h"

#include <stdexcept>

IlluminationDrawList::IlluminationDrawList(DrawListUptr drawList,
                                           const std::vector<LightSptr> &lights)
    : DrawListDecorator(std::move(drawList)), m_lights(lights) {

  _drawList->getShader()->setInt("nLights", static_cast<int>(lights.size()));

  if (ResourceManager<Camera>::FindOrError("main", m_camera) != ERROR_OK)
    throw std::runtime_error("An error occurred while constructing IlluminationDrawList.");
}

void IlluminationDrawList::draw(ERROR &errCode) {
  ShaderSptr _shader = _drawList->getShader();
  _shader->use();

  // Apply all lights to the shader
  for (size_t i = 0; i < m_lights.size(); i++) {
    m_lights[i]->use(*_shader, i);

    // Apply light space matrix if this is a shadow-casting light
    if (m_lights[i]->isShadowCaster())
      m_lights[i]->setLightSpaceMatrix(*_shader);
  }

  _shader->setMat4("projection", m_camera->getProjection());
  _shader->setMat4("view", m_camera->getView());
  _shader->setVec3("viewPos", m_camera->getPosition());

  DrawListDecorator::draw(errCode);
}