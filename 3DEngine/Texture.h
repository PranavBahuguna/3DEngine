#pragma once

#include "Error.h"
#include "Resource.h"

#include <memory>

#include <GL/glew.h>

#define ERROR_TEXTURE "Textures/error.jpg"

class Texture;
using TexSptr = std::shared_ptr<Texture>;

class Texture : public Resource {
public:
  Texture(const std::string &filename);
  ~Texture();

  void use() const;

  int getWidth() const { return m_width; };
  int getHeight() const { return m_height; };

  static GLenum GetFormat(ERROR &errCode, int channels);

private:
  ERROR load(const std::string &filepath);

  GLuint m_textureID;
  int m_width;
  int m_height;
};
