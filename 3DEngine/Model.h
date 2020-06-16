#pragma once

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

static unsigned int id = 0;

class Model {
public:
  Model(const std::string &name);

  unsigned int _id;

  ERROR load();

  void applyLight(const Light &light, ERROR &errCode) const;
  void draw(const Camera &camera, ERROR &errCode) const;
  void setPosition(const glm::vec3 &pos);
  void setRotation(const glm::vec3 &euler, float angle);
  void setScale(const glm::vec3 &scale);

  glm::mat4 getMatrix() const;

  virtual void update(ERROR &errCode) {}

protected:
  virtual void loadAssets(const aiScene &scene, ERROR &errCode);

  void loadMeshes(const aiNode &node, const aiScene &scene, ERROR &errCode);
  void loadMaterials(const aiScene &scene, ERROR &errCode);

  void getMeshProperties(const aiMesh &mesh, std::vector<GLfloat> &verts,
                         std::vector<GLfloat> &texCoordss, std::vector<GLfloat> &normals,
                         std::vector<GLuint> &indices) const;

  std::string m_name;

  glm::vec3 m_pos;
  glm::vec3 m_euler;
  float m_angle;
  glm::vec3 m_scale;

  std::vector<std::shared_ptr<Mesh>> m_meshes;
  std::vector<std::shared_ptr<Texture>> m_textures;
  std::vector<std::shared_ptr<Material>> m_materials;
  std::vector<unsigned int> m_meshToTex;
  std::shared_ptr<Shader> m_shader;
};