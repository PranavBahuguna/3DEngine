#include "TransparencyDrawList.h"

#include <algorithm>
#include <stdexcept>

#include <glm/gtx/norm.hpp>

// Constructor
TransparencyDrawList::TransparencyDrawList(DrawListUptr drawList)
    : DrawListDecorator(std::move(drawList)) {

  if (ResourceManager<Camera>::FindOrError("main", m_camera) != ERROR_OK)
    throw std::runtime_error("An error occurred while constructing TransparencyDrawList.");
}

void TransparencyDrawList::draw(ERROR &errCode) {
  // Sort the draw list by order of distance from the camera descending. This ensures that
  // targets furthest away from the camera are not occluded by other targets in front.
  auto &_targets = getTargets();
  glm::vec3 camPos = m_camera->getPosition();

  std::sort(_targets.begin(), _targets.end(), [camPos](const DrawSptr &t1, const DrawSptr &t2) {
    float dist1 = glm::distance2(camPos, t1->getPos());
    float dist2 = glm::distance2(camPos, t2->getPos());
    return dist1 > dist2;
  });

  ShaderSptr _shader = getShader();
  _shader->use();
  _shader->setMat4("view", m_camera->getView());
  _shader->setMat4("projection", m_camera->getProjection());

  DrawListDecorator::draw(errCode);
}
