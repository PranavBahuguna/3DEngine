#pragma once

#include "Error.h"

#include <GL/glew.h>

class Texture {
public:
  Texture(const std::string &filename);
  ~Texture();

  void use() const;

  int getWidth() const { return m_width; };
  int getHeight() const { return m_height; };

private:
  ERROR load(const std::string &filepath);
  void setFormat(int channels, ERROR &errCode);

  GLuint m_textureID;
  GLenum m_format;
  int m_width;
  int m_height;
  int m_bitDepth;
};