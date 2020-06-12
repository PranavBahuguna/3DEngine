#pragma once

#include "Error.h"
#include "Font.h"
#include "Shader.h"

#include <memory>

class Text {
public:
  Text(const std::string &font, const glm::vec2 &pos, const GLfloat scale, const glm::vec3 &color, const glm::mat4 &projection);

  void draw(const std::string &str, ERROR &errCode) const;

private:
  std::shared_ptr<Shader> m_shader;
  std::shared_ptr<Font> m_font;
  glm::vec2 m_pos;
  GLfloat m_scale;
  glm::vec3 m_color;

  GLuint m_VAO;
  GLuint m_VBO;
};