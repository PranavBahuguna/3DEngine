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
  const glm::vec3 &getPos() const;
  const glm::vec3 &getEuler() const;
  const glm::vec3 &getScale() const;
  void setPos(const glm::vec3 &pos);
  void setEuler(const glm::vec3 &euler);
  void setScale(const glm::vec3 &scale);
  void move(const glm::vec3 &moveVec);
  void rotate(const glm::vec3 &euler);
  void scale(const glm::vec3 &scale);

private:
  ModelSptr m_model;
  ScriptSptr m_script;
};