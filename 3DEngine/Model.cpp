#include "Model.h"

// Constructor
Model::Model(const std::string &name)
    : m_name(name), m_pos(0.0f), m_angle(0.0f), m_euler(1.0f), m_scale(1.0f) {

  m_shader = Resources::GetShader("Triangle");

  static unsigned int id = 0;
  _id = id++;
}

// Applies a light source to the model
void Model::applyLight(const Light &light, ERROR &errCode) const { light.use(*m_shader, errCode); }

// Draws the model to the screen
void Model::draw(const Camera &camera, ERROR &errCode) const {
  if (errCode != ERROR_OK)
    return;

  // Check that we have a mesh and shader to use
  if (m_meshes.empty())
    errCode = ERROR_MISSING_MESH;
  else if (!m_shader)
    errCode = ERROR_MISSING_SHADER;

  if (errCode != ERROR_OK) {
    printErrorMsg(errCode);
    return;
  }

  // Set shader parameters and apply
  m_shader->use();
  GLuint modelId = m_shader->getParamId("model", errCode);
  GLuint viewId = m_shader->getParamId("view", errCode);
  GLuint projId = m_shader->getParamId("projection", errCode);
  GLuint viewPos = m_shader->getParamId("viewPos", errCode);

  if (errCode != ERROR_OK)
    return;

  glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(getMatrix()));
  glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(camera.getView()));
  glUniformMatrix4fv(projId, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));
  glUniform3f(viewPos, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

  // Iterate over each stored mesh/texture/material and draw
  for (size_t i = 0; i < m_meshes.size() && errCode == ERROR_OK; i++) {
    unsigned int materialIndex = m_meshToTex[i];

    if (materialIndex < m_textures.size() && m_textures[materialIndex] != nullptr)
      m_textures[materialIndex]->use();

    // Apply mesh material and draw
    m_materials[i]->use(*m_shader, errCode);
    m_meshes[i]->draw();
  }
}

// Set model position
void Model::setPosition(const glm::vec3 &pos) { m_pos = pos; }

// Set model rotation and angle
void Model::setRotation(const glm::vec3 &euler, float angle) {
  m_euler = euler;
  m_angle = angle;
}

// Set model scale
void Model::setScale(const glm::vec3 &scale) { m_scale = scale; }

// Gets the model matrix
glm::mat4 Model::getMatrix() const {
  auto matrix = glm::mat4(1.0f);
  matrix = glm::translate(matrix, m_pos);
  matrix = glm::rotate(matrix, glm::radians(m_angle), m_euler);
  matrix = glm::scale(matrix, m_scale);

  return matrix;
}

// Loads model from the given file path
ERROR Model::load() {
  ERROR errCode = ERROR_OK;

  const std::string filename = "Models/" + m_name + ".obj";

  // Setup importer and try reading the scene data
  Assimp::Importer importer;
  unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                       aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials;
  const aiScene *scene = importer.ReadFile(filename, flags);

  if (scene == nullptr) {
    errCode = ERROR_MODEL_LOAD_FAILED;
    printErrorMsg(errCode, filename.c_str(), importer.GetErrorString());
  } else {
    loadAssets(*scene, errCode);
  }

  return errCode;
}

// Loads all relevant assets for this model
void Model::loadAssets(const aiScene &scene, ERROR &errCode) {
  loadMeshes(*scene.mRootNode, scene, errCode);
  loadMaterials(scene, errCode);
}

// Loads all meshes from the current node
void Model::loadMeshes(const aiNode &node, const aiScene &scene, ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  // Iterate and load each mesh
  for (size_t i = 0; i < node.mNumMeshes; i++) {
    aiMesh *mesh = scene.mMeshes[node.mMeshes[i]];

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> texCoords;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;
    getMeshProperties(*mesh, vertices, texCoords, normals, indices);

    std::string meshName = m_name + "_" + std::to_string(mesh->mMaterialIndex);
    m_meshes.push_back(Resources::GetMesh(meshName, vertices, texCoords, normals, indices));
    m_meshToTex.push_back(mesh->mMaterialIndex);
  }

  // Recursively load from all child nodes
  for (size_t i = 0; i < node.mNumChildren; i++)
    loadMeshes(*node.mChildren[i], scene, errCode);
}

// Load all materials from the current scene
void Model::loadMaterials(const aiScene &scene, ERROR &errCode) {

  m_textures.resize(scene.mNumMaterials);

  for (size_t i = 0; i < scene.mNumMaterials; i++) {
    aiMaterial *material = scene.mMaterials[i];
    m_textures[i] = nullptr;

    // Load textures associated with the material
    if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
      aiString path;

      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
        size_t idx = std::string(path.data).rfind("\\");
        std::string filename = std::string(path.data).substr(idx + 1);

        m_textures[i] = Resources::GetTexture(filename);
      }
    }

    // If no texture associated with the material, treat it as having an opacity texture
    if (m_textures[i] == nullptr)
      m_textures[i] = Resources::GetTexture("opacity.png");

    // Load the material itself
    m_materials.push_back(Resources::GetMaterial(*material));
  }
}

// Generate the mesh from the assimp mesh object
void Model::getMeshProperties(const aiMesh &mesh, std::vector<GLfloat> &verts,
                              std::vector<GLfloat> &texCoords, std::vector<GLfloat> &normals,
                              std::vector<GLuint> &indices) const {

  for (size_t i = 0; i < mesh.mNumVertices; i++) {
    // Vertices
    verts.push_back(mesh.mVertices[i].x);
    verts.push_back(mesh.mVertices[i].y);
    verts.push_back(mesh.mVertices[i].z);

    // Texture coordinates
    if (mesh.mTextureCoords[0]) {
      texCoords.push_back(mesh.mTextureCoords[0][i].x);
      texCoords.push_back(mesh.mTextureCoords[0][i].y);
    } else {
      texCoords.insert(texCoords.end(), {0.0f, 0.0f});
    }

    // Normals
    normals.push_back(mesh.mNormals[i].x);
    normals.push_back(mesh.mNormals[i].y);
    normals.push_back(mesh.mNormals[i].z);
  }

  // Find all mesh indices
  for (size_t i = 0; i < mesh.mNumFaces; i++) {
    aiFace face = mesh.mFaces[i];

    for (size_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
}