#include "Text.h"
#include "ResourceManager.h"

// Constructor
Text::Text(const Transform &transform, const std::string &font, float scale, const glm::vec4 &color,
           const std::string &text)
    : Drawable(transform), m_font(ResourceManager<Font>::Get(font)), m_scale(scale), m_color(color),
      m_text(text) {

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

// Draws a text string onto the screen
void Text::draw(ERROR &errCode, const Shader &shader) const {
  // Pass shader parameters and activate
  shader.setVec4("textColor", m_color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(m_VAO);

  // Starting x/y position current character in text string
  float xStart = m_transform.getPosition().x;
  float yStart = m_transform.getPosition().y;

  // Iterate and draw each character
  for (const char &c : m_text) {
    const auto &ch = m_font->getCharacter(errCode, c);
    if (errCode != ERROR_OK)
      break;

    // Calculate position and size for the given character
    float xPos = xStart + ch->bearing.x * m_scale;
    float yPos = yStart - (ch->size.y - ch->bearing.y) * m_scale;
    float w = ch->size.x * m_scale;
    float h = ch->size.y * m_scale;

    // Construct vertices and bind data to VBO
    float vertices[4][4] = {
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