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