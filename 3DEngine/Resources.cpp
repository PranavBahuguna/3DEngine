#include "Resources.h"

std::shared_ptr<Mesh> Resources::GetMesh(const std::string &name,
                                         const std::vector<GLfloat> &vertices,
                                         const std::vector<GLfloat> &texCoords,
                                         const std::vector<GLfloat> &normals,
                                         const std::vector<GLuint> &indices) {
  auto &mesh = meshMap[name];
  if (mesh.expired()) {
    std::shared_ptr<Mesh> newMesh(new Mesh(vertices, texCoords, normals, indices));
    mesh = newMesh;
    return newMesh;
  } else {
    return mesh.lock();
  }
}

std::shared_ptr<Shader> Resources::GetShader(const std::string &name) {
  auto &shader = shaderMap[name];
  if (shader.expired()) {
    std::shared_ptr<Shader> newShader(new Shader(name));
    shader = newShader;
    return newShader;
  } else {
    return shader.lock();
  }
}

std::shared_ptr<Texture> Resources::GetTexture(const std::string &name) {
  auto &texture = textureMap[name];
  if (texture.expired()) {
    std::shared_ptr<Texture> newTexture(new Texture(name));
    texture = newTexture;
    return newTexture;
  } else {
    return texture.lock();
  }
}

std::shared_ptr<Material> Resources::GetMaterial(const std::string &name, const glm::vec3 &ambient,
                                                 const glm::vec3 &diffuse,
                                                 const glm::vec3 &specular, GLfloat shininess) {
  auto &material = materialMap[name];
  if (material.expired()) {
    std::shared_ptr<Material> newMaterial(new Material(ambient, diffuse, specular, shininess));
    material = newMaterial;
    return newMaterial;
  } else {
    return material.lock();
  }
}

std::shared_ptr<Font> Resources::GetFont(const std::string &name) {
  auto &font = fontMap[name];
  if (font.expired()) {
    std::shared_ptr<Font> newFont(new Font(name));
    font = newFont;
    return newFont;
  } else {
    return font.lock();
  }
}