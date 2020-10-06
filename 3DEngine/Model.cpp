#include "Model.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

Model::Model(const std::string &name) : m_name(name), m_euler(0.0f), m_scale(1.0f) {
  if (ResourceManager<Texture>::FindOrError("depth-map", m_depthTexture))
    throw std::runtime_error("An error occurred while initializing Model.");
}

// Loads model from the given file path
void Model::load(ERROR &errCode) {
  AssetLoader::loadMeshes(errCode, m_name, m_meshes, m_materials, m_textures);
}

// Draws the model to the screen
void Model::draw(ERROR &errCode, const Shader &shader) const {
  // Check that we have a mesh to use
  if (m_meshes.empty()) {
    errCode = printErrorMsg(ERROR_MISSING_MESH);
    return;
  }

  // Set shader parameters and apply
  shader.setMat4("model", getMatrix());

  if (shader._name == "Lighting") {
    // Apply depth texture
    m_depthTexture->use(GL_TEXTURE1);

    // Iterate over each stored mesh/texture/material and draw
    for (size_t i = 0; i < m_meshes.size(); i++) {
      if (i < m_textures.size() && m_textures[i] != nullptr)
        m_textures[i]->use();

      // Apply mesh material and draw
      m_materials[i]->use(shader);
    }
  }

  for (const auto &mesh : m_meshes)
    mesh->draw();
}

// Gets the model matrix
glm::mat4 Model::getMatrix() const {
  auto matrix = glm::mat4(1.0f);
  matrix = glm::translate(matrix, m_pos);
  matrix = glm::rotate(matrix, m_euler.x, {1.0f, 0.0f, 0.0f});
  matrix = glm::rotate(matrix, m_euler.y, {0.0f, 1.0f, 0.0f});
  matrix = glm::rotate(matrix, m_euler.z, {0.0f, 0.0f, 1.0f});
  matrix = glm::scale(matrix, m_scale);

  return matrix;
}