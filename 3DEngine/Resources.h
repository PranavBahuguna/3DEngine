#pragma once

#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

#include <assimp/material.h>
#include <assimp/mesh.h>

template <typename T> using ResourceMap = std::unordered_map<std::string, std::weak_ptr<T>>;

static ResourceMap<Mesh> meshMap;
static ResourceMap<Shader> shaderMap;
static ResourceMap<Texture> textureMap;
static ResourceMap<Material> materialMap;
static ResourceMap<Font> fontMap;

class Resources {
public:
  static bool findMesh(const std::string &name) { return meshMap.find(name) != meshMap.end(); }
  static bool findShader(const std::string &name) {
    return shaderMap.find(name) != shaderMap.end();
  }
  static bool findTexture(const std::string &name) {
    return textureMap.find(name) != textureMap.end();
  }
  static bool findMaterial(const std::string &name) {
    return materialMap.find(name) != materialMap.end();
  }
  static bool findFont(const std::string &name) { return fontMap.find(name) != fontMap.end(); }

  static std::shared_ptr<Mesh> GetMesh(const std::string &name,
                                       const std::vector<GLfloat> &vertices,
                                       const std::vector<GLfloat> &texCoords,
                                       const std::vector<GLfloat> &normals,
                                       const std::vector<GLuint> &indices);
  static std::shared_ptr<Shader> GetShader(const std::string &name);
  static std::shared_ptr<Texture> GetTexture(const std::string &name);
  static std::shared_ptr<Material> GetMaterial(const aiMaterial &mat);
  static std::shared_ptr<Font> GetFont(const std::string &name);

private:
  Resources() {} // prevent construction of this class
};
