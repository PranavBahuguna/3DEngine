#pragma once

#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

template <typename T> using ResourceMap = std::unordered_map<std::string, std::weak_ptr<T>>;

static ResourceMap<Mesh> meshMap;
static ResourceMap<Shader> shaderMap;
static ResourceMap<Texture> textureMap;
static ResourceMap<Material> materialMap;
static ResourceMap<Font> fontMap;

class Resources {
public:
  static std::shared_ptr<Mesh> GetMesh(const std::string &name,
                                       const std::vector<GLfloat> &vertices,
                                       const std::vector<GLfloat> &texCoords,
                                       const std::vector<GLfloat> &normals,
                                       const std::vector<GLuint> &indices);
  static std::shared_ptr<Shader> GetShader(const std::string &name);
  static std::shared_ptr<Texture> GetTexture(const std::string &name);
  static std::shared_ptr<Material> GetMaterial(const std::string &name, const glm::vec3 &ambient,
                                               const glm::vec3 &diffuse, const glm::vec3 &specular,
                                               GLfloat shininess);
  static std::shared_ptr<Font> GetFont(const std::string &name);

private:
  Resources() {} // prevent construction of this class
};
