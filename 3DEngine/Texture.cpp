#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

#include <stdexcept>

// Constructor
Texture::Texture(const std::string &filename) : Resource{filename} {
  // Get file path and try loading texture
  const std::string path = "Textures/" + filename;

  if (load(path) != ERROR_OK) {
    // Try loading the error texture instead
    if (load(ERROR_TEXTURE) != ERROR_OK)
      throw std::runtime_error("An error occurred while loading texture.");
  }
}

// Destructor - clear texture bound to id
Texture::~Texture() { glDeleteTextures(1, &m_textureID); }

// Loads texture from given file path
ERROR Texture::load(const std::string &filepath) {
  // Try loading the texture from path
  int bitDepth = 0;
  unsigned char *texData = stbi_load(filepath.c_str(), &m_width, &m_height, &bitDepth, 0);
  if (texData == nullptr)
    return printErrorMsg(ERROR_FILE_LOAD_FAILED, filepath.c_str());

  // Set the texture format
  ERROR errCode = ERROR_OK;
  GLenum format = GetFormat(errCode, bitDepth);

  if (errCode == ERROR_OK) {
    // Bind the texture and set its properties
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, texData);
  }
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

// Enable usage of this texture
void Texture::use() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}
