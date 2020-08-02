#include "AssetLoader.h"

#include <algorithm>
#include <fstream>

#define MODELS_DIR "Models/"
#define MATERIALS_DIR "Materials/"

// Load a mesh from .obj file with it's associated materials and textures
void AssetLoader::loadMeshes(ERROR &errCode, const std::string &name, AssetPtrs<Mesh> &meshes,
                             AssetPtrs<Material> &materials, AssetPtrs<Texture> &textures) {
  std::string filename = MODELS_DIR + name + ".obj";
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> mats;
  std::string warn;
  std::string err;

  bool ret =
      tinyobj::LoadObj(&attrib, &shapes, &mats, &warn, &err, filename.c_str(), MATERIALS_DIR);

  // Notify of any errors in trying to load the model
  bool success = warn.empty() && err.empty() && ret;
  if (!success) {
    printf(warn.c_str());
    printf(err.c_str());
    errCode = printErrorMsg(ERROR_FILE_LOAD_FAILED, filename.c_str());
    return;
  }

  buildMeshes(meshes, name, attrib, shapes, mats);
  buildMaterials(materials, textures, mats);
}

// Load materials from .mtl file with any associated textures only
void AssetLoader::loadMaterials(ERROR &errCode, const std::string &name,
                                AssetPtrs<Material> &materials, AssetPtrs<Texture> &textures) {
  std::string filename = MATERIALS_DIR + name + ".mtl";
  std::map<std::string, int> matMap;
  std::vector<tinyobj::material_t> mats;
  std::ifstream matIStream(filename.c_str());
  std::string warn;
  std::string err;

  tinyobj::LoadMtl(&matMap, &mats, &matIStream, &warn, &err);

  // Notify of any errors in trying to load the model
  bool success = warn.empty() && err.empty();
  if (!success) {
    printf(warn.c_str());
    printf(err.c_str());
    errCode = printErrorMsg(ERROR_FILE_LOAD_FAILED, filename.c_str());
    return;
  }

  buildMaterials(materials, textures, mats);
}

// Construct meshes based on data loaded from .obj file
void AssetLoader::buildMeshes(AssetPtrs<Mesh> &meshes, const std::string &name,
                              const tinyobj::attrib_t &attrib,
                              const std::vector<tinyobj::shape_t> &shapes,
                              const std::vector<tinyobj::material_t> &mats) {
  // Meshes are split by material
  size_t index_offset = 0;
  const auto &fullMesh = shapes[0].mesh;
  unsigned int nMeshes =
      *std::max_element(fullMesh.material_ids.begin(), fullMesh.material_ids.end()) + 1;

  std::vector<std::vector<GLfloat>> positions(nMeshes);
  std::vector<std::vector<GLfloat>> normals(nMeshes);
  std::vector<std::vector<GLfloat>> texCoords(nMeshes);
  std::vector<std::vector<GLuint>> indices(nMeshes);

  // Loop over each face
  for (size_t f = 0; f < fullMesh.num_face_vertices.size(); f++) {
    int fv = fullMesh.num_face_vertices[f];
    int mIndex = fullMesh.material_ids[f];

    for (size_t v = 0; v < fv; v++) {
      const auto &index = fullMesh.indices[index_offset + v];

      positions[mIndex].push_back(attrib.vertices[3 * index.vertex_index + 0]);
      positions[mIndex].push_back(attrib.vertices[3 * index.vertex_index + 1]);
      positions[mIndex].push_back(attrib.vertices[3 * index.vertex_index + 2]);

      normals[mIndex].push_back(attrib.normals[3 * index.normal_index + 0]);
      normals[mIndex].push_back(attrib.normals[3 * index.normal_index + 1]);
      normals[mIndex].push_back(attrib.normals[3 * index.normal_index + 2]);

      texCoords[mIndex].push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
      texCoords[mIndex].push_back(attrib.texcoords[2 * index.texcoord_index + 1]);

      indices[mIndex].push_back((GLuint)indices[mIndex].size());
    }
    index_offset += fv;
  }

  meshes.resize(nMeshes);
  for (size_t i = 0; i < nMeshes; i++) {
    std::string meshName = name + "_" + std::to_string(i);

    // Check if this mesh already exists
    meshes[i] = Resources::FindMesh(meshName);
    if (meshes[i] == nullptr)
      meshes[i] =
          Resources::CreateMesh(meshName, positions[i], texCoords[i], normals[i], indices[i]);
  }
}

// Construct materials and textures based on data loaded from .mtl file
void AssetLoader::buildMaterials(AssetPtrs<Material> &materials, AssetPtrs<Texture> &textures,
                                 const std::vector<tinyobj::material_t> &mats) {
  textures.resize(mats.size());
  materials.resize(mats.size());

  for (size_t i = 0; i < mats.size(); i++) {
    const auto &mat = mats[i];
    textures[i] = nullptr;

    // Load textures associated with the material
    if (!mat.diffuse_texname.empty())
      textures[i] = Resources::GetTexture(mat.diffuse_texname);

    // If no texture associated with the material, treat it as having an opacity texture
    if (textures[i] == nullptr)
      textures[i] = Resources::GetTexture("opacity.png");

    // Load the material itself - check if this material already exists
    materials[i] = Resources::FindMaterial(mat.name);

    if (materials[i] == nullptr) {
      // Create the material instead
      const auto &ambient = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
      const auto &diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
      const auto &specular = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);

      materials[i] = Resources::CreateMaterial(mat.name, ambient, diffuse, specular, mat.shininess);
    }
  }
}