#include "LightIcon.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#define DIRLIGHT_ICON_POS_HEIGHT 5.0f
#define DIRLIGHT_ICON_OFFSET_FACTOR 3.0f
#define LIGHT_ICON_SIZE 256.0f
#define LIGHT_ICON_IMAGE "light-icons.png"

// Constructor
LightIcon::LightIcon(LightSptr light)
    : m_light(light), m_texture(ResourceManager<Texture>::Get(LIGHT_ICON_IMAGE)) {
  // Position for directional lights is in a fixed location (with offset for light direction).
  // Position for other lights is at light location.
  const auto &lightType = m_light->getType();
  glm::vec3 lightPos = glm::vec3(m_light->getPosition());

  if (lightType == LightType::DIRECTIONAL_LIGHT)
    m_pos = glm::normalize(lightPos) * DIRLIGHT_ICON_OFFSET_FACTOR +
            glm::vec3(0.0f, DIRLIGHT_ICON_POS_HEIGHT, 0.0f);
  else
    m_pos = lightPos;

  // Get the appropriate icon from the sprite map
  glm::vec2 texIndices = {0, 0};

  switch (lightType) {
  case LightType::DIRECTIONAL_LIGHT:
    texIndices = {0, 0}; // sun
    break;
  case LightType::POINT_LIGHT:
    texIndices = {0, 1}; // light bulb
    break;
  case LightType::SPOT_LIGHT:
    texIndices = {1, 0}; // flashlight
    break;
  }

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
  shader.setVec4("color", glm::vec4(m_light->getColor(), 1.0f));
  shader.setMat4("model", glm::translate(glm::mat4(1.0f), m_pos));

  // Use texture and draw vertices
  m_texture->use();

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}