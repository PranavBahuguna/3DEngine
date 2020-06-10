#pragma once

#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

#include <assimp/mesh.h>
#include <assimp/material.h>

class Resources {
public:
  static std::shared_ptr<Mesh> GetMesh(const aiMesh &aiMesh);
  static std::shared_ptr<Shader> GetShader(const std::string &name);
  static std::shared_ptr<Texture> GetTexture(const std::string &name);
  static std::shared_ptr<Material> GetMaterial(const aiMaterial &mat);
  static std::shared_ptr<Font> GetFont(const std::string &name);

private:
  Resources() {} // prevent construction of this class
};
