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
  Texture(const std::string &name, GLsizei width, GLsizei height, const void *texData,
          GLenum format, GLenum type, bool generateMipmaps = false);
  Texture(const std::string &name, GLsizei width, GLsizei height, const void *texData,
          GLenum format, GLenum internalFormat, GLenum type, bool generateMipmaps = false);
  Texture(const std::string &filename);
  ~Texture();

  void create(const void *texData, GLenum format, GLenum internalFormat, GLenum type,
              bool generateMipmaps);
  void setParameter(GLuint param, int value);
  void setParameter(GLuint param, float value);
  void setParameter(GLuint param, int *values);
  void setParameter(GLuint param, float *values);
  void use() const;

  int getWidth() const { return m_width; };
  int getHeight() const { return m_height; };
  GLuint getId() const { return m_id; }

  static GLenum GetFormat(ERROR &errCode, int channels);

private:
  ERROR load(const std::string &filepath);

  GLuint m_id;
  int m_width;
  int m_height;
};
