#include "Model.h"

// Constructor
Model::Model()
    : m_pos(glm::vec3(0.0f)), m_angle(0.0f), m_euler(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)),
      m_mesh(nullptr) {

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
  if (!m_mesh)
    errCode = ERROR_MISSING_MESH;
  else if (!m_shader)
    errCode = ERROR_MISSING_SHADER;

  if (errCode != ERROR_OK) {
    printErrorMsg(errCode);
    return;
  }

  // Apply shader (and texture if applicable)
  m_shader->use();
  if (m_texture)
    m_texture->use();

  // Set shader parameters
  glm::mat4 model = getMatrix();
  m_shader->setModel(glm::transpose(glm::inverse(model)));
  glm::mat4 mvp = camera.getProjection() * camera.getView() * model;
  m_shader->setMVP(mvp);
  m_shader->setViewPos(camera.getPosition());

  // Apply mesh material and draw
  m_mesh->getMaterial()->use(*m_shader);
  m_mesh->draw();
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
