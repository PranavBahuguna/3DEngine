#include "Text.h"
#include "Resources.h"

#include <stdexcept>

// Constructor
Text::Text(const std::string &font, const glm::vec2 &pos, const GLfloat scale,
           const glm::vec4 &color, const glm::vec2 &screenDimensions)
    : m_scale(scale), m_color(color) {

  m_font = Resources::GetFont(font);
  m_shader = Resources::GetShader("Text");

  // Convert from relative to absolute position
  m_pos.x = pos.x * screenDimensions.x;
  m_pos.y = pos.y * screenDimensions.y;

  // Setup shader properties
  ERROR errCode = ERROR_OK;
  m_shader->use();

  const glm::mat4 projection =
      glm::ortho(0.0f, (GLfloat)screenDimensions.x, 0.0f, (GLfloat)screenDimensions.y, 0.0f, 1.0f);
  glUniformMatrix4fv(m_shader->getParamId("projection", errCode), 1, GL_FALSE,
                     glm::value_ptr(projection));
  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while creating Text object.");

  // Configure VAO / VBO for texture quads and bind buffer data
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  }
}

// Sets this text object's text string
void Text::setText(const std::string &text) { m_text = text; }

// Draws a text string onto the screen
void Text::draw(ERROR &errCode) const {
  // Activate shader and pass parameters
  m_shader->use();
  glUniform4f(m_shader->getParamId("textColor", errCode), m_color.r, m_color.g, m_color.b,
              m_color.a);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(m_VAO);

  GLfloat xStart = m_pos.x; // starting x-position current character in text string

  // Iterate and draw each character
  for (const char &c : m_text) {
    const Character *ch = m_font->getCharacter(c, errCode);

    // Calculate position and size for the given character
    GLfloat xPos = xStart + ch->bearing.x * m_scale;
    GLfloat yPos = m_pos.y - (ch->size.y - ch->bearing.y) * m_scale;
    GLfloat w = ch->size.x * m_scale;
    GLfloat h = ch->size.y * m_scale;

    // Construct vertices and bind data to VBO
    GLfloat vertices[4][4] = {
        {xPos, yPos, 0.0f, 1.0f},
        {xPos + w, yPos, 1.0f, 1.0f},
        {xPos, yPos + h, 0.0f, 0.0f},
        {xPos + w, yPos + h, 1.0f, 0.0f},
    };

    glBindTexture(GL_TEXTURE_2D, ch->textureId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Advance x-position for next glyph (bitshift by 6 to get advance value in pixels)
    xStart += (ch->advance >> 6) * m_scale;
  }
}