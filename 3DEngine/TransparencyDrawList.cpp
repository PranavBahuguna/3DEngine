#include "TransparencyDrawList.h"
#include "Game.h"

#include <algorithm>
#include <stdexcept>

#include <glm/gtx/norm.hpp>

TransparencyDrawList::TransparencyDrawList(DrawListUptr drawList)
    : DrawListDecorator(std::move(drawList)) {}

void TransparencyDrawList::draw(ERROR &errCode) {
  // Sort the draw list by order of distance from the camera descending. This ensures that
  // targets furthest away from the camera are not occluded by other targets in front.
  auto &targets = getTargets();
  glm::vec3 camPos = Game::GetCamera().getPosition();

  std::sort(targets.begin(), targets.end(), [camPos](const DrawSptr &t1, const DrawSptr &t2) {
    float dist1 = glm::distance2(camPos, t1->transform().getPosition());
    float dist2 = glm::distance2(camPos, t2->transform().getPosition());
    return dist1 > dist2;
  });

  ShaderSptr shader = getShader();
  shader->setMat4("view", Game::GetCamera().getView());
  shader->setMat4("projection", Game::GetCamera().getProjection());

  DrawListDecorator::draw(errCode);
}
