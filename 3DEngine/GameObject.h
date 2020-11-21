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
  Transform &getTransform();

private:
  ModelSptr m_model;
  ScriptSptr m_script;
};