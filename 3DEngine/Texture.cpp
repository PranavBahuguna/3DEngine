#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

#include <stdexcept>

// Constructor - direct creation
Texture::Texture(const std::string &name, GLsizei width, GLsizei height, const void *texData,
                 GLenum format, GLenum internalFormat, GLenum type, bool generateMipmaps)
    : Resource{name}, m_width(width), m_height(height) {
  create(texData, format, internalFormat, type, generateMipmaps);
}

// Constructor - direct creation (format and internal format are same)
Texture::Texture(const std::string &name, GLsizei width, GLsizei height, const void *texData,
                 GLenum format, GLenum type, bool generateMipmaps)
    : Texture(name, width, height, texData, format, format, type, generateMipmaps) {}

// Constructor - load from file path
Texture::Texture(const std::string &filename) : Resource{filename} {
  // Get file path and try loading texture
  const std::string path = "Textures/" + filename;

  if (load(path) != ERROR_OK) {
    // Try loading the error texture instead
    if (load(ERROR_TEXTURE) != ERROR_OK)
      throw std::runtime_error("An error occurred while loading texture.");
  }
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

// Create a new texture
void Texture::create(const void *texData, GLenum format, GLenum internalFormat, GLenum type,
                     bool generateMipmaps) {
  // Generate and bind texture
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, type, texData);

  if (generateMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

// Texture parameter setter methods - requires this texture to be bound first
void Texture::setParameter(GLuint param, int value) {
  glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::setParameter(GLuint param, float value) {
  glTexParameterf(GL_TEXTURE_2D, param, value);
}

void Texture::setParameter(GLuint param, int *values) {
  glTexParameteriv(GL_TEXTURE_2D, param, values);
}

void Texture::setParameter(GLuint param, float *values) {
  glTexParameterfv(GL_TEXTURE_2D, param, values);
}

// Enable usage of this texture
void Texture::use(GLuint slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

// Loads texture from given file path
ERROR Texture::load(const std::string &filepath) {
  // Try loading the texture from path
  int bitDepth = 0;
  unsigned char *texData = stbi_load(filepath.c_str(), &m_width, &m_height, &bitDepth, 0);
  if (texData == nullptr)
    return printErrorMsg(ERROR_FILE_LOAD_FAILED, filepath.c_str());

  // Get the texture format
  ERROR errCode = ERROR_OK;
  GLenum format = GetFormat(errCode, bitDepth);

  // Create the texture
  if (errCode == ERROR_OK)
    create(texData, format, format, GL_UNSIGNED_BYTE, true);

  // Set parameters
  setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
  setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(texData);

  return errCode;
}

// Sets the texture format based on the number of channels found (bit depth)
GLenum Texture::GetFormat(ERROR &errCode, int channels) {
  // Number of channels must be between 1 and 4
  GLenum format = 0;
  if (channels >= 1 && channels <= 4) {
    // clang-format off
    switch (channels) {
      case 1: format = GL_RED;  break;
      case 2: format = GL_RG;   break;
      case 3: format = GL_RGB;  break;
      case 4: format = GL_RGBA; break;
    }
    // clang-format on
  } else {
    errCode = printErrorMsg(ERROR_INVALID_BITDEPTH, channels);
  }

  return format;
}