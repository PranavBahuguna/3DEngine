#pragma once

#include "tiny_obj_loader.h"

#include "Error.h"
#include "Resources.h"

template <typename T> using AssetPtrs = std::vector<std::shared_ptr<T>>;

class AssetLoader {
public:
  static void loadMeshes(const std::string &name, AssetPtrs<Mesh> &meshes,
                         AssetPtrs<Material> &materials, AssetPtrs<Texture> &textures,
                         ERROR &errCode);
  static void loadMaterials(const std::string &name, AssetPtrs<Material> &materials,
                            AssetPtrs<Texture> &textures, ERROR &errCode);

private:
  static void buildMeshes(AssetPtrs<Mesh> &meshes, const std::string &name,
                          const tinyobj::attrib_t &attrib,
                          const std::vector<tinyobj::shape_t> &shapes,
                          const std::vector<tinyobj::material_t> &mats);
  static void buildMaterials(AssetPtrs<Material> &materials, AssetPtrs<Texture> &textures,
                             const std::vector<tinyobj::material_t> &mats);

  AssetLoader() {} // prevent construction of this class
};