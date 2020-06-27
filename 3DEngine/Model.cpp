#include "Model.h"

// Constructor
Model::Model(const std::string &name)
    : m_name(name), m_pos(0.0f), m_angle(0.0f), m_euler(1.0f), m_scale(1.0f) {

  m_shader = Resources::GetShader("Lighting");

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
    size_t materialIndex = i;

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
void Model::load(ERROR &errCode) {
  AssetLoader::loadMeshes(m_name, m_meshes, m_materials, m_textures, errCode);
}