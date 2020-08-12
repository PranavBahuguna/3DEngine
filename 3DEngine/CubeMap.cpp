#include "CubeMap.h"
#include "stb_image.h"

#include <stdexcept>

#define CUBEMAP_DIR "Textures/Cubemaps/"

// Constructor - loads all image files from folder name and extension only
CubeMap::CubeMap(const std::string &folder, const std::string &extension)
    : CubeMap(folder, {CUBEMAP_DIR + folder + "/posx" + extension,
                       CUBEMAP_DIR + folder + "/negx" + extension,
                       CUBEMAP_DIR + folder + "/posy" + extension,
                       CUBEMAP_DIR + folder + "/negy" + extension,
                       CUBEMAP_DIR + folder + "/posz" + extension,
                       CUBEMAP_DIR + folder + "/negz" + extension}) {}

// Constructor - supply a name for the collection and each image file individually
CubeMap::CubeMap(const std::string &name, const std::vector<std::string> &files) : Resource{name} {
  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

  // Try loading each available image file
  for (GLuint i = 0; i < 6; i++) {
    if (i >= files.size() || load(files[i], i) != ERROR_OK) {
      // Try loading the error texture instead
      if (load(ERROR_TEXTURE, i) != ERROR_OK)
        throw std::runtime_error("An error occurred while loading texture.");
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

// Destructor - clear texture bound to id
CubeMap::~CubeMap() { glDeleteTextures(1, &m_textureID); }

// Loads texture from given file path for a specific cube map face
ERROR CubeMap::load(const std::string &filepath, GLuint faceIndex) {
  // Try loading the texture from path
  int bitDepth, width, height = 0;
  unsigned char *texData = stbi_load(filepath.c_str(), &width, &height, &bitDepth, 0);
  if (texData == nullptr)
    return printErrorMsg(ERROR_FILE_LOAD_FAILED, filepath.c_str());

  ERROR errCode = ERROR_OK;
  GLenum format = Texture::GetFormat(errCode, bitDepth);

  if (errCode == ERROR_OK)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, texData);
  stbi_image_free(texData);

  return errCode;
}

// Enable usage of this cubemap
void CubeMap::use() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}
