#pragma once

#include "Drawable.h"
#include "Font.h"

class Text;
using TextSptr = std::shared_ptr<Text>;

class Text : public Drawable {
public:
  Text(const std::string &font, const glm::vec2 &pos, const float scale, const glm::vec4 &color,
       const std::string &text = "");

  void draw(ERROR &errCode, const Shader &shader) const override;
  void setText(const std::string &text) { m_text = text; }

private:
  std::string m_text;
  FontSptr m_font;
  float m_scale;
  glm::vec4 m_color;

  GLuint m_VAO;
  GLuint m_VBO;
};