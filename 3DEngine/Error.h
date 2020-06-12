#pragma once

#include <cstdint>
#include <initializer_list>
#include <stdarg.h>
#include <string>
#include <unordered_map>

typedef uint32_t ERROR;

// clang-format off
#define ERROR_OK                                0
#define ERROR_GLFW_INIT_FAILED                  1
#define ERROR_GLFW_WINDOW_CREATE_FAILED         2
#define ERROR_GLEW_INIT_FAILED                  3
#define ERROR_FILE_OPEN_FAILED                  4
#define ERROR_SHADER_COMPILE_FAILED             5
#define ERROR_SHADER_PROGRAM_CREATE_FAILED      6
#define ERROR_SHADER_PROGRAM_INVALID            7
#define ERROR_SHADER_PROGRAM_LINKING_FAILED     8
#define ERROR_INVALID_BITDEPTH                  9
#define ERROR_MISSING_MESH                      10
#define ERROR_MISSING_SHADER                    11
#define ERROR_SHADER_MISSING_PARAMETER          12
#define ERROR_MODEL_LOAD_FAILED                 13
#define ERROR_CANNOT_INITIALISE_FREETYPE        14
#define ERROR_FONT_LOAD_FAILED                  15
#define ERROR_FONT_GLYPH_LOAD_FAILED            16
#define ERROR_FONT_CHARACTER_OUT_OF_RANGE       17
// clang-format on

static void printErrorMsg(ERROR errCode, ...) {

  std::string errHeader = "Error " + std::to_string(errCode) + " - ";
  std::string errBody;

  switch (errCode) {
  case ERROR_GLFW_INIT_FAILED:
    errBody = "GLFW window creation failed!";
    break;
  case ERROR_GLFW_WINDOW_CREATE_FAILED:
    errBody = "GLFW window creation failed!";
    break;
  case ERROR_GLEW_INIT_FAILED:
    errBody = "GLEW initialization failed!";
    break;
  case ERROR_FILE_OPEN_FAILED:
    errBody = "Unable to open file: %s";
    break;
  case ERROR_SHADER_COMPILE_FAILED:
    errBody = "Unable to compile shader %d!\nSource: %s";
    break;
  case ERROR_SHADER_PROGRAM_CREATE_FAILED:
    errBody = "Unable to create shader program %d!";
    break;
  case ERROR_SHADER_PROGRAM_INVALID:
    errBody = "Error in validating program:\n%s";
    break;
  case ERROR_SHADER_PROGRAM_LINKING_FAILED:
    errBody = "Error in linking program:\n%s";
    break;
  case ERROR_INVALID_BITDEPTH:
    errBody = "Bitdepth of %d found during texture load is invalid! Bitdepth must be between 1 "
              "and 4 inclusive.";
    break;
  case ERROR_MISSING_MESH:
    errBody = "A mesh has not been assigned to this object.";
    break;
  case ERROR_MISSING_SHADER:
    errBody = "A shader has not been assigned to this object.";
    break;
  case ERROR_SHADER_MISSING_PARAMETER:
    errBody = "Could not locate parameter (%s) in shader program.";
    break;
  case ERROR_MODEL_LOAD_FAILED:
    errBody = "Failed to load wavefront file (%s): %s";
    break;
  case ERROR_CANNOT_INITIALISE_FREETYPE:
    errBody = "Could not initialise the FreeType library: FT error %d - %s";
    break;
  case ERROR_FONT_LOAD_FAILED:
    errBody = "Failed to load font (%s): FT error %d - %s";
    break;
  case ERROR_FONT_GLYPH_LOAD_FAILED:
    errBody = "Failed to load glyph %d from font (%s): FT error %d - %s";
    break;
  case ERROR_FONT_CHARACTER_OUT_OF_RANGE:
    errBody = "Character (%s) is not contained in font (%s).";
    break;
  default:
    errBody = "Unknown error";
    break;
  }

  std::string errMsg = errHeader + errBody + "\n";

  // Add all variadic arguments to be printed
  va_list args;
  va_start(args, errCode);
  vprintf(errMsg.c_str(), args);
  va_end(args);
}
