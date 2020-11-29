#pragma once

#include "Drawable.h"
#include "Font.h"

class Text;
using TextSptr = std::shared_ptr<Text>;

class Text : public Drawable {
public:
  Text(const Transform &transform, const std::string &font, float scale, const glm::vec4 &color,
       const std::string &text = "");

  void draw(ERROR &errCode, const Shader &shader) const override;
  void setText(const std::string &text) { m_text = text; }

private:
  Transform m_transform;
  std::string m_text;
  FontSptr m_font;
  float m_scale;
  glm::vec4 m_color;

  GLuint m_VAO;
  GLuint m_VBO;
};