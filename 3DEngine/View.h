#pragma once

#include "Transform.h"

#include <glm/gtx/quaternion.hpp>

class View {
public:
  View(const Transform &transform);

  View &setPosition(const glm::vec3 &position);
  View &translate(const glm::vec3 &translation);
  View &setRotation(const glm::vec3 &rotation);
  View &rotate(const glm::vec3 &rotation);

  const glm::mat4 &getView();
  const glm::quat &getOrientation();
  const glm::vec3 &getPosition();
  const glm::vec3 &getRotation();
  glm::vec3 getFront();
  glm::vec3 getRight();
  glm::vec3 getUp();

private:
  void updateView();
  void updateOrientation();

  Transform m_transform;
  glm::mat4 m_viewMatrix;
  glm::quat m_orientation;
};