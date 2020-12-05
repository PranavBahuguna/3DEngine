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
  const auto &cameraTransform = Game::GetCamera().transform();
  const auto &cameraProjection = Game::GetCamera().projection();

  glm::vec3 camPos = cameraTransform.getPosition();
  std::sort(targets.begin(), targets.end(),
            [camPos](const std::shared_ptr<GameObject> &t1, const std::shared_ptr<GameObject> &t2) {
              float dist1 = glm::distance2(camPos, t1->GetComponent<Transform>()->getPosition());
              float dist2 = glm::distance2(camPos, t2->GetComponent<Transform>()->getPosition());
              return dist1 > dist2;
            });

  ShaderSptr shader = getShader();
  shader->setMat4("view", cameraTransform.getView());
  shader->setMat4("projection", cameraProjection.getProjection());

  DrawListDecorator::draw(errCode);
}
