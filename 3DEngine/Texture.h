#pragma once

#include "Error.h"
#include "Resource.h"

#include <memory>

#include <GL/glew.h>

class Texture;
using TexSptr = std::shared_ptr<Texture>;

class Texture : public Resource {
public:
  Texture(const std::string &filename);
  ~Texture();

  void use() const;

  int getWidth() const { return m_width; };
  int getHeight() const { return m_height; };

private:
  ERROR load(const std::string &filepath);
  void setFormat(ERROR &errCode, int channels);

  GLuint m_textureID;
  GLenum m_format;
  int m_width;
  int m_height;
  int m_bitDepth;
};