#include "LightIcon.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#define DIRLIGHT_ICON_POS_HEIGHT 5.0f
#define DIRLIGHT_ICON_OFFSET_FACTOR 3.0f
#define LIGHT_ICON_SIZE 256.0f
#define LIGHT_ICON_IMAGE "light-icons.png"

LightIcon::LightIcon(LightSptr light) : Drawable(light->transform()) {
  glm::vec2 texIndices = {1, 1};
  m_lightColor = glm::vec4(light->getColor(), 1.0f);

  if (dynamic_cast<DirectionalLight *>(light.get()) != nullptr) {
    // Position for directional lights is in a fixed location (with offset for light direction),
    // rather than at light location itself
    m_transform.setPosition(m_transform.getFront() * DIRLIGHT_ICON_OFFSET_FACTOR +
                            glm::vec3(0.0f, DIRLIGHT_ICON_POS_HEIGHT, 0.0f));
    texIndices = {0, 0};
  } else if (dynamic_cast<PointLight *>(light.get()) != nullptr) {
    texIndices = {0, 1};
  } else if (dynamic_cast<SpotLight *>(light.get()) != nullptr) {
    texIndices = {1, 0};
  }

  m_texture = ResourceManager<Texture>::Get(LIGHT_ICON_IMAGE);

  // Calculate icon texture coordinates and set them accordingly
  GLfloat texW = LIGHT_ICON_SIZE / m_texture->getWidth();
  GLfloat texH = LIGHT_ICON_SIZE / m_texture->getHeight();

  GLfloat texX = texIndices.x * texW;
  GLfloat texY = texIndices.y * texH;

  // Configure VAO / VBO for texture quads and bind buffer data
  GLfloat vertices[4][4] = {
      {0, 0, texX, texY + texH},        // bottom-left
      {1, 0, texX + texW, texY + texH}, // top-left
      {0, 1, texX, texY},               // bottom-right
      {1, 1, texX + texW, texY}         // top-right
  };

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

// Draws a light icon onto the screen
void LightIcon::draw(ERROR &errCode, const Shader &shader) const {
  // Pass shader parameters
  shader.setVec4("color", m_lightColor);
  shader.setMat4("model", glm::translate(glm::mat4(1.0f), m_transform.getPosition()));

  // Use texture and draw vertices
  m_texture->use();

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}