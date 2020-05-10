#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

class Resources {
public:
  static std::shared_ptr<Mesh> GetMesh(const std::string &name);
  static std::shared_ptr<Shader> GetShader(const std::string &name);
  static std::shared_ptr<Texture> GetTexture(const std::string &name);
  static std::shared_ptr<Material> GetMaterial(const tinyobj::material_t &mat);

private:
  Resources() {} // prevent construction of this class
};
