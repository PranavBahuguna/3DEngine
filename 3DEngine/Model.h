#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
  Model(const std::string &name);
  virtual ~Model(){};

  unsigned int _id;

  void LoadModel(const std::string &filename);

  virtual void update(ERROR &errCode) {}
  void applyLight(const Light &light) const;
  void draw(const Camera &camera, ERROR &errCode) const;

  void setPosition(const glm::vec3 &pos);
  void setRotation(const glm::vec3 &euler, float angle);
  void setScale(const glm::vec3 &scale);

  glm::mat4 getMatrix() const;

protected:
  glm::vec3 m_pos;
  glm::vec3 m_euler;
  float m_angle;
  glm::vec3 m_scale;

  void LoadNode(aiNode *node, const aiScene *scene);
  void LoadMaterials(const aiScene *scene);

  std::vector<std::shared_ptr<Mesh>> m_meshes;
  std::vector<std::shared_ptr<Texture>> m_textures;
  std::vector<std::shared_ptr<Material>> m_materials;
  std::vector<unsigned int> m_meshToTex;
  std::shared_ptr<Shader> m_shader;
};