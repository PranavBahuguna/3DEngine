#pragma once

#include "AssetLoader.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static unsigned int id = 0;

class Model {
public:
  Model(const std::string &name);

  unsigned int _id;

  virtual void load(ERROR &errCode);
  virtual void update(ERROR &errCode, GLfloat deltaTime) {}

  void applyLight(const Light &light, ERROR &errCode) const;
  void draw(const Camera &camera, ERROR &errCode) const;
  void setPosition(const glm::vec3 &pos);
  void setRotation(const glm::vec3 &euler, float angle);
  void setScale(const glm::vec3 &scale);

  glm::mat4 getMatrix() const;

protected:
  std::string m_name;

  glm::vec3 m_pos;
  glm::vec3 m_euler;
  float m_angle;
  glm::vec3 m_scale;

  AssetPtrs<Mesh> m_meshes;
  AssetPtrs<Texture> m_textures;
  AssetPtrs<Material> m_materials;
  std::shared_ptr<Shader> m_shader;
};