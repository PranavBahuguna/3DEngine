#pragma once

#include <glm/glm.hpp>

class Transform {
public:
  Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);

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

private:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
};
