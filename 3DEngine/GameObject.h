#pragma once

#include "Model.h"
#include "Script.h"

// Forward declarations
class Script;

using ModelSptr = std::shared_ptr<Model>;
using ScriptSptr = std::shared_ptr<Script>;

class GameObject {
public:
  GameObject(ModelSptr model);
  GameObject(ModelSptr model, const std::string &scriptName);

  void init(ERROR &errCode);
  void update(ERROR &errCode);

  // Model properties getters / setters
  const glm::vec3 &getPos() { return m_model->transform().getPosition(); }
  const glm::vec3 &getEuler() { return glm::radians(m_model->transform().getRotation()); }
  const glm::vec3 &getScale() { return m_model->transform().getScale(); }

  void setPos(const glm::vec3 &pos) { m_model->transform().setPosition(pos); }
  void setEuler(const glm::vec3 &euler) { m_model->transform().setRotation(glm::radians(euler)); }
  void setScale(const glm::vec3 &scale) { m_model->transform().setScale(scale); }

  void move(const glm::vec3 &moveVec) { m_model->transform().translate(moveVec); }
  void rotate(const glm::vec3 &euler) { m_model->transform().rotate(glm::radians(euler)); }
  void scale(const glm::vec3 &scale) { m_model->transform().reScale(scale); }

private:
  ModelSptr m_model;
  ScriptSptr m_script;
};