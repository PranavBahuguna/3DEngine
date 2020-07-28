#pragma once

#include "Model.h"
#include "Script.h"

// Forward declarations
class Script;

using ModelPtr = std::shared_ptr<Model>;
using ScriptPtr = std::shared_ptr<Script>;

class GameObject {
public:
  GameObject(ModelPtr model);
  GameObject(ModelPtr model, const std::string &scriptName);

  void setModel(const ModelPtr model) { m_model = model; }
  void loadScript(const std::string &name);
  void init(ERROR &errCode);
  void update(ERROR &errCode);

  // Model properties getters / setters
  const glm::vec3 &getPos() { return m_model->getPos(); }
  const glm::vec3 &getEuler() { return m_model->getEuler(); }
  const glm::vec3 &getScale() { return m_model->getScale(); }

  void setPos(const glm::vec3 &pos) { m_model->setPos(pos); }
  void setEuler(const glm::vec3 &euler) { m_model->setEuler(euler); }
  void setScale(const glm::vec3 &scale) { m_model->setScale(scale); }

  void move(const glm::vec3 &moveVec) { m_model->move(moveVec); }
  void rotate(const glm::vec3 &euler) { m_model->rotate(euler); }
  void scale(const glm::vec3 &scale) { m_model->scale(scale); }

private:
  ModelPtr m_model;
  ScriptPtr m_script;
};