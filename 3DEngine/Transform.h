#pragma once

#include <glm/glm.hpp>

class Transform {
public:
  Transform(const glm::vec3 &position = glm::vec3(), const glm::vec3 &rotation = glm::vec3(),
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
  glm::mat4 getModel() const;

  void limitRotation();

private:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
};
