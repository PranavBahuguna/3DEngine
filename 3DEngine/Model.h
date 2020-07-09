#pragma once

#include "AssetLoader.h"
#include "Drawable.h"
#include "Light.h"
#include "Shader.h"

class Model : public Drawable {
public:
  Model(const std::string &name);

  virtual void load(ERROR &errCode);
  virtual void update(ERROR &errCode) {}

  void draw(const Shader &shader, ERROR &errCode) const override;
  void setPosition(const glm::vec3 &pos);
  void setRotation(const glm::vec3 &euler, float angle);
  void setScale(const glm::vec3 &scale);

protected:
  glm::mat4 getMatrix() const;

  std::string m_name;
  glm::vec3 m_euler;
  float m_angle;
  glm::vec3 m_scale;

  AssetPtrs<Mesh> m_meshes;
  AssetPtrs<Texture> m_textures;
  AssetPtrs<Material> m_materials;
};