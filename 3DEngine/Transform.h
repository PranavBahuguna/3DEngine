#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform : public Component {
public:
  Transform(const glm::vec3 &position = glm::vec3(), const glm::vec3 &rotation = glm::vec3(),
            const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));
  Transform(const std::shared_ptr<GameObject> &owner, const glm::vec3 &position = glm::vec3(),
            const glm::vec3 &rotation = glm::vec3(),
            const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));

  // All mutator methods return the current transform to allow for method chaining
  Transform &setPosition(const glm::vec3 &position);
  Transform &translate(const glm::vec3 &translation);
  Transform &setRotation(const glm::vec3 &rotation);
  Transform &rotate(const glm::vec3 &rotation);
  Transform &setScale(const glm::vec3 &scale);
  Transform &reScale(const glm::vec3 &scale);

  const glm::vec3 &getPosition() const;
  const glm::vec3 &getRotation() const;
  const glm::vec3 &getScale() const;
  const glm::quat &getOrientation() const;
  const glm::mat4 &getModel() const;
  const glm::mat4 &getView() const;
  glm::vec3 getFront() const;
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;

private:
  void limitRotation(glm::vec3 &rotation) const;
  void updateOrientation() const;
  void updateModel() const;
  void updateView() const;

  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;

  mutable glm::quat m_orientation;
  mutable glm::mat4 m_model;
  mutable glm::mat4 m_view;
  mutable bool m_orientationUpdated;
  mutable bool m_modelUpdated;
  mutable bool m_viewUpdated;
};
