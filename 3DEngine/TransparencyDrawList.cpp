#include "TransparencyDrawList.h"

#include <algorithm>

#include <glm/gtx/norm.hpp>

// Constructor
TransparencyDrawList::TransparencyDrawList(DListPtr drawList)
    : DrawListDecorator(std::move(drawList)) {}

void TransparencyDrawList::draw(ERROR &errCode) {
  // Sort the draw list by order of distance from the camera descending. This ensures that
  // targets furthest away from the camera are not occluded by other targets in front.
  auto &_targets = getTargets();
  std::sort(_targets.begin(), _targets.end(), [](const DrawPtr &t1, const DrawPtr &t2) {
    float dist1 = glm::distance2(Camera::GetPosition(), t1->getPos());
    float dist2 = glm::distance2(Camera::GetPosition(), t2->getPos());
    return dist1 > dist2;
  });

  ShaderPtr _shader = getShader();
  _shader->use();
  _shader->setMat4("view", Camera::GetView(), errCode);
  _shader->setMat4("projection", Camera::GetProjection(), errCode);

  DrawListDecorator::draw(errCode);
}