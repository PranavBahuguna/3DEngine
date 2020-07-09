#pragma once

#include "Drawable.h"
#include "Resources.h"

#include <memory>

class Text : public Drawable {
public:
  Text(const std::string &font, const glm::vec2 &pos, const GLfloat scale, const glm::vec4 &color);

  void draw(const Shader &shader, ERROR &errCode) const override;
  void setText(const std::string &text) { m_text = text; }

private:
  std::string m_text;
  FontPtr m_font;
  GLfloat m_scale;
  glm::vec4 m_color;

  GLuint m_VAO;
  GLuint m_VBO;
};