#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

// Constructor
Model::Model(const std::string &name)
    : m_name(name), m_pos(0.0f), m_angle(0.0f), m_euler(1.0f), m_scale(1.0f) {

  static unsigned int id = 0;
  _id = id++;
}

// Draws the model to the screen
void Model::draw(const Shader &shader, ERROR &errCode) const {
  // Check that we have a mesh to use
  if (m_meshes.empty()) {
    errCode = ERROR_MISSING_MESH;
    printErrorMsg(errCode);
  }

  // Set shader parameters and apply
  shader.setMat4("model", getMatrix(), errCode);

  // Iterate over each stored mesh/texture/material and draw
  for (size_t i = 0; i < m_meshes.size() && errCode == ERROR_OK; i++) {
    size_t materialIndex = i;

    if (materialIndex < m_textures.size() && m_textures[materialIndex] != nullptr)
      m_textures[materialIndex]->use();

    // Apply mesh material and draw
    m_materials[i]->use(shader, errCode);
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
void Model::load(ERROR &errCode) {
  AssetLoader::loadMeshes(m_name, m_meshes, m_materials, m_textures, errCode);
}