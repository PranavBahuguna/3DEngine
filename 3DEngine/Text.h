#pragma once

#include "Error.h"
#include "Font.h"
#include "Shader.h"

#include <memory>

class Text {
public:
  Text(const std::string &font, const glm::vec2 &pos, const GLfloat scale, const glm::vec4 &color);

  void setText(const std::string &text);
  void draw(const Shader &shader, ERROR &errCode) const;

private:
  std::string m_text;
  std::shared_ptr<Font> m_font;
  glm::vec2 m_pos;
  GLfloat m_scale;
  glm::vec4 m_color;

  GLuint m_VAO;
  GLuint m_VBO;
};