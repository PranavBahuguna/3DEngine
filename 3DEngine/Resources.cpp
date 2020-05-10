#include "Resources.h"

std::shared_ptr<Mesh> Resources::GetMesh(const std::string &name) {
  static std::unordered_map<std::string, std::weak_ptr<Mesh>> map;
  std::weak_ptr<Mesh> &mesh = map[name];
  if (mesh.expired()) {
    std::shared_ptr<Mesh> newMesh(new Mesh(name));
    mesh = newMesh;
    return newMesh;
  } else {
    return mesh.lock();
  }
}

std::shared_ptr<Shader> Resources::GetShader(const std::string& name) {
  static std::unordered_map<std::string, std::weak_ptr<Shader>> map;
  std::weak_ptr<Shader> &shader = map[name];
  if (shader.expired()) {
    std::shared_ptr<Shader> newShader(new Shader(name));
    shader = newShader;
    return newShader;
  } else {
    return shader.lock();
  }
}

std::shared_ptr<Texture> Resources::GetTexture(const std::string& name) {
  static std::unordered_map<std::string, std::weak_ptr<Texture>> map;
  std::weak_ptr<Texture> &texture = map[name];
  if (texture.expired()) {
    std::shared_ptr<Texture> newTexture(new Texture(name));
    texture = newTexture;
    return newTexture;
  } else {
    return texture.lock();
  }
}

std::shared_ptr<Material> Resources::GetMaterial(const tinyobj::material_t &mat) {
  static std::unordered_map<std::string, std::weak_ptr<Material>> map;
  std::weak_ptr<Material> &material = map[mat.name];
  if (material.expired()) {
    auto ambient = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
    auto diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
    auto specular = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
    std::shared_ptr<Material> newMaterial(new Material(ambient, diffuse, specular, mat.shininess));
    material = newMaterial;
    return newMaterial;
  } else {
    return material.lock();
  }
}