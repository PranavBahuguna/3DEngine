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

  const glm::vec3 &getPos() { return m_pos; }
  const glm::vec3 &getEuler() { return m_euler; }
  float getAngle() { return m_angle; }
  const glm::vec3 &getScale() { return m_scale; }

  void setPos(const glm::vec3 &pos) { m_pos = pos; }
  void setEuler(const glm::vec3 &euler) { m_euler = euler; }
  void setAngle(float angle) { m_angle = angle; }
  void setScale(const glm::vec3 &scale) { m_scale = scale; }

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