#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

#include <stdexcept>

// Constructor
Texture::Texture(const std::string &filename)
    : m_textureID(0), m_width(0), m_height(0), m_bitDepth(0), m_filename(filename) {

  // Get file path and try loading texture
  const std::string path = "Textures/" + filename;

  if (load(path) != ERROR_OK)
    throw std::runtime_error("An error occurred while loading texture.");
}

// Destructor - clear texture bound to id
Texture::~Texture() { glDeleteTextures(1, &m_textureID); }

// Loads texture from given file path
ERROR Texture::load(const std::string &filepath) {
  ERROR errCode = ERROR_OK;

  // Try loading the texture from path
  unsigned char *texData = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bitDepth, 0);
  if (!texData) {
    errCode = ERROR_FILE_OPEN_FAILED;
    printErrorMsg(errCode, filepath.c_str());
  }

  // Set the texture format
  if (errCode == ERROR_OK)
    errCode = setFormat(m_bitDepth);

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
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE,
                 texData);
  }
  stbi_image_free(texData);

  return errCode;
}

// Sets the texture format based on the number of channels found (bit depth)
ERROR Texture::setFormat(int channels) {
  ERROR errCode = ERROR_OK;

  // Number of channels must be between 1 and 4
  if (channels >= 1 && channels <= 4) {
    // clang-format off
    switch (channels) {
      case 1: m_format = GL_RED;  break;
      case 2: m_format = GL_RG;   break;
      case 3: m_format = GL_RGB;  break;
      case 4: m_format = GL_RGBA; break;
    }
    // clang-format on
  } else {
    errCode = ERROR_INVALID_BITDEPTH;
    printErrorMsg(errCode, channels);
  }

  return errCode;
}

// Enable usage of this texture
void Texture::use() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}
