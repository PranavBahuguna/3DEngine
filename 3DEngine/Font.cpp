#include "Font.h"

#include <stdexcept>

#define DEFAULT_FONT "Fonts/Calibri.ttf"
#define FONT_PIXEL_SIZE 64

// Constructor
Font::Font(const std::string &name) : m_name(name), m_characters{} {
  ERROR errCode = ERROR_OK;

  // Try initialising the FreeType library
  FT_Library ft;
  FT_Error ftErrCode = FT_Init_FreeType(&ft);
  if (ftErrCode != FT_Err_Ok) {
    errCode = printErrorMsg(ERROR_CANNOT_INITIALISE_FREETYPE, ftErrCode, getFTErrorMsg(ftErrCode));
  } else {
    // Get file path and try loading font
    const std::string path = "Fonts/" + name + ".ttf";

    if (load(path, ft) != ERROR_OK)
      errCode = load(DEFAULT_FONT, ft); // try loading default font instead
  }

  if (errCode != ERROR_OK)
    throw std::runtime_error("An error occurred while loading font.");
}

// Loads font from the given file path
ERROR Font::load(const std::string &filepath, FT_Library &ft) {
  // Try loading font face
  FT_Face face;
  FT_Error ftErrCode = FT_New_Face(ft, filepath.c_str(), 0, &face);
  if (ftErrCode != FT_Err_Ok)
    return printErrorMsg(ERROR_FONT_LOAD_FAILED, m_name.c_str(), ftErrCode,
                         getFTErrorMsg(ftErrCode));

  // Set font pixel size
  ftErrCode = FT_Set_Pixel_Sizes(face, 0, FONT_PIXEL_SIZE);
  if (ftErrCode != FT_Err_Ok)
    return printErrorMsg(ERROR_FONT_LOAD_FAILED, m_name.c_str(), ftErrCode,
                         getFTErrorMsg(ftErrCode));

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

  ERROR errCode = ERROR_OK;
  for (unsigned char c = 0; c < CHAR_ARRAY_SIZE; c++) {
    // Load character glyph
    ftErrCode = FT_Load_Char(face, c, FT_LOAD_RENDER);
    if (ftErrCode != FT_Err_Ok) {
      errCode = printErrorMsg(ERROR_FONT_GLYPH_LOAD_FAILED, (int)c, m_name.c_str(), ftErrCode,
                              getFTErrorMsg(ftErrCode));
      continue;
    }

    // Generate texture for glyph
    const auto &bitmap = face->glyph->bitmap;
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 bitmap.buffer);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Store character info in array
    m_characters[c] = {textureId, glm::ivec2(bitmap.width, bitmap.rows),
                       glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                       face->glyph->advance.x};
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return errCode;
}

// Retrieves font character from store
const Character *Font::getCharacter(ERROR &errCode, const unsigned char c) const {
  if (c >= CHAR_ARRAY_SIZE) {
    errCode = printErrorMsg(ERROR_FONT_CHARACTER_OUT_OF_RANGE, c, m_name.c_str());
    return nullptr;
  }

  return &m_characters[c];
}

// Helper method for getting error messages from the FreeType library
const char *Font::getFTErrorMsg(const FT_Error &err) {
#undef FTERRORS_H_
#define FT_ERRORDEF(e, v, s)                                                                       \
  case e:                                                                                          \
    return s;
#define FT_ERROR_START_LIST switch (err) {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H
  return "(Unknown error)";
}