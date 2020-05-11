#include "Model.h"

// Constructor
Model::Model(const std::string &name)
    : m_pos(glm::vec3(0.0f)), m_angle(0.0f), m_euler(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {

  const std::string filename = "Models/" + name + ".obj";

  LoadModel(filename);
  m_shader = Resources::GetShader("Triangle");

  static unsigned int id = 0;
  _id = id++;
}

// Applies a light source to the model
void Model::applyLight(const Light &light) const { light.use(*m_shader); }

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

  // Apply shader (and texture if applicable)
  m_shader->use();
  if (!m_textures.empty())
    m_textures.front()->use();

  // Set shader parameters
  glm::mat4 model = getMatrix();
  m_shader->setModel(glm::transpose(glm::inverse(model)));
  glm::mat4 mvp = camera.getProjection() * camera.getView() * model;
  m_shader->setMVP(mvp);
  m_shader->setViewPos(camera.getPosition());

  // Apply mesh material and draw
  m_materials.front()->use(*m_shader);
  m_meshes.front()->draw();
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

////////////////////////////////////////////////////////////////////////////////////////////////////

// Loads model from the given file path
void Model::LoadModel(const std::string &filename) {
  // Setup importer and try reading the scene data
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

  if (!scene) {
    printf("ERROR");
    return;
  }

  LoadNode(scene->mRootNode, scene);
  LoadMaterials(scene);
}

// Loads all meshes from the current node
void Model::LoadNode(aiNode *node, const aiScene *scene) {
  // Iterate and load each mesh
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_meshes.push_back(Resources::GetMesh(mesh));
    m_meshToTex.push_back(mesh->mMaterialIndex);
  }

  // Recursively load from all child nodes
  for (size_t i = 0; i < node->mNumChildren; i++)
    LoadNode(node->mChildren[i], scene);
}

// Load all materials from the current scene
void Model::LoadMaterials(const aiScene *scene) {

  for (size_t i = 0; i < scene->mNumMaterials; i++) {
    aiMaterial *material = scene->mMaterials[i];

    // Load textures associated with the material
    if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
      aiString path;
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
        size_t idx = std::string(path.data).rfind("\\");
        std::string filename = std::string(path.data).substr(idx + 1);

        m_textures.push_back(Resources::GetTexture(filename));
      }
    }

    // Load the material itself
    m_materials.push_back(Resources::GetMaterial(material));
  }
}