#pragma once

#include "Model.h"
#include "Script.h"

// Forward declarations
class Script;

using ModelPtr = std::shared_ptr<Model>;
using ScriptPtr = std::shared_ptr<Script>;

class GameObject {
public:
  GameObject(const std::string &name);

  const std::string _name;

  void setModel(const ModelPtr model) { m_model = model; }
  void loadScript(const std::string &name, ERROR &errCode);
  void init(ERROR &errCode);
  void update(ERROR &errCode);

  float getAngle() { return m_model->getAngle(); }
  void setAngle(float angle) { m_model->setAngle(angle); }

private:
  ModelPtr m_model;
  ScriptPtr m_script;
};