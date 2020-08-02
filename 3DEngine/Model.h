#pragma once

#include "AssetLoader.h"
#include "Drawable.h"
#include "Light.h"
#include "Shader.h"

class Model : public Drawable {
public:
  Model(const std::string &name);

  virtual void load(ERROR &errCode);

  void draw(ERROR &errCode, const Shader &shader) const override;

  const glm::vec3 &getPos() { return m_pos; }
  const glm::vec3 &getEuler() { return m_euler; }
  const glm::vec3 &getScale() { return m_scale; }

  void setPos(const glm::vec3 &pos) { m_pos = pos; }
  void setEuler(const glm::vec3 &euler) { m_euler = glm::radians(euler); }
  void setScale(const glm::vec3 &scale) { m_scale = scale; }

  void move(const glm::vec3 &moveVec) { m_pos += moveVec; }
  void rotate(const glm::vec3 &euler) { m_euler += glm::radians(euler); }
  void scale(const glm::vec3 &scale) { m_scale += scale; }

protected:
  glm::mat4 getMatrix() const;

  std::string m_name;
  glm::vec3 m_euler;
  glm::vec3 m_scale;

  AssetPtrs<Mesh> m_meshes;
  AssetPtrs<Texture> m_textures;
  AssetPtrs<Material> m_materials;
};