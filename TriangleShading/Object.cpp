#include "Object.h"

// Constructor
Object::Object()
    : m_pos(glm::vec3(0.0f)), m_angle(0.0f), m_euler(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)),
      m_mesh(nullptr), m_shader(nullptr), m_texture(nullptr) {

  static unsigned int id = 0;
  _id = id++;
}

// Destructor
Object::~Object() {
  if (m_mesh)
    delete m_mesh;
  if (m_shader)
    delete m_shader;
  if (m_texture)
    delete m_texture;
}

// Draws the object to the screen
ERROR Object::draw(const Camera &camera) {
  // Check that we have a mesh and shader to use
  ERROR errCode;
  errCode = m_mesh ? ERROR_OK : ERROR_MISSING_MESH;
  errCode = m_shader ? ERROR_OK : ERROR_MISSING_SHADER;
  if (errCode != ERROR_OK)
    return errCode;

  // Calculate the MVP matrix and draw
  const glm::mat4 mvp = camera.getProjection() * camera.getView() * getModel();
  m_shader->setMVP(mvp);
  m_mesh->draw();

  // Apply shader (and texture if applicable)
  m_shader->use();
  if (m_texture)
    m_texture->use();

  return errCode;
}

// Gets the object model
glm::mat4 Object::getModel() const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_pos);
  model = glm::rotate(model, glm::radians(m_angle), m_euler);
  model = glm::scale(model, m_scale);

  return model;
}
