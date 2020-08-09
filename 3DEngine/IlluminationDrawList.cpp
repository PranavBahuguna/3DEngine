#include "IlluminationDrawList.h"

// Constructor
IlluminationDrawList::IlluminationDrawList(DrawListUptr drawList)
    : DrawListDecorator(std::move(drawList)) {}

void IlluminationDrawList::setLights(const std::vector<std::string> &lightNames) {
  for (const auto &name : lightNames) {
    LightSptr light;
    if (Resources<Light>::Find(name, light))
      m_lights.push_back(light);
  }
}

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