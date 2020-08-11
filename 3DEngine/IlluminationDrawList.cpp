#include "IlluminationDrawList.h"

// Constructor
IlluminationDrawList::IlluminationDrawList(DrawListUptr drawList,
                                           const std::vector<LightSptr> &lights)
    : DrawListDecorator(std::move(drawList)), m_lights(lights) {}

void IlluminationDrawList::draw(ERROR &errCode) {
  ShaderSptr _shader = _drawList->getShader();
  _shader->use();

  // Apply all lights to the shader
  for (size_t i = 0; i < m_lights.size(); i++)
    m_lights[i]->use(*_shader, i);

  _shader->setMat4("projection", Camera::GetProjection());
  _shader->setMat4("view", Camera::GetView());
  _shader->setVec3("viewPos", Camera::GetPosition());

  DrawListDecorator::draw(errCode);
}