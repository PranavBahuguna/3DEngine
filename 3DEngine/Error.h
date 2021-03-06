#pragma once

#include <cstdint>
#include <initializer_list>
#include <stdarg.h>
#include <string>

typedef uint32_t ERROR;

// clang-format off
#define ERROR_OK                                0
#define ERROR_GLFW_INIT_FAILED                  1
#define ERROR_GLFW_WINDOW_CREATE_FAILED         2
#define ERROR_GLEW_INIT_FAILED                  3
#define ERROR_FILE_LOAD_FAILED                  4
#define ERROR_SHADER_COMPILE_FAILED             5
#define ERROR_SHADER_PROGRAM_CREATE_FAILED      6
#define ERROR_SHADER_PROGRAM_INVALID            7
#define ERROR_SHADER_PROGRAM_LINKING_FAILED     8
#define ERROR_SHADER_PREPROCESSOR_NOT_FOUND     9
#define ERROR_INVALID_BITDEPTH                  10
#define ERROR_MISSING_MESH                      11
#define ERROR_SHADER_MISSING_PARAMETER          12
#define ERROR_CANNOT_INITIALISE_FREETYPE        13
#define ERROR_FONT_LOAD_FAILED                  14
#define ERROR_FONT_GLYPH_LOAD_FAILED            15
#define ERROR_FONT_CHARACTER_OUT_OF_RANGE       16
#define ERROR_INPUT_KEY_OUT_OF_RANGE            17
#define ERROR_LUA_ERROR                         18
#define ERROR_RESOURCE_NOT_FOUND                19
// clang-format on

// If error reference is not required for a function, this macro may be used as a placeholder
#define error_placeholder details::placeholder(ERROR())

namespace details {
inline ERROR &placeholder(ERROR &&e) { return e; }
} // namespace details

static ERROR printErrorMsg(ERROR errCode, ...) {
  // ERROR_OK has no associated message to print
  if (errCode == ERROR_OK)
    return errCode;

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
  case ERROR_FILE_LOAD_FAILED:
    errBody = "An error occurred while loading file: %s";
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
  case ERROR_SHADER_PREPROCESSOR_NOT_FOUND:
    errBody = "Preprocessor (%s) was not found in shader (%s).";
    break;
  case ERROR_INVALID_BITDEPTH:
    errBody = "Bitdepth of %d found during texture load is invalid! Bitdepth must be between 1 "
              "and 4 inclusive.";
    break;
  case ERROR_MISSING_MESH:
    errBody = "A mesh has not been assigned to this object.";
    break;
  case ERROR_SHADER_MISSING_PARAMETER:
    errBody = "Could not locate parameter (%s) in shader program.";
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
  case ERROR_INPUT_KEY_OUT_OF_RANGE:
    errBody = "Input key (%d) is out of range.";
    break;
  case ERROR_LUA_ERROR:
    errBody = "Lua error: %s";
    break;
  case ERROR_RESOURCE_NOT_FOUND:
    errBody = "Resource (%s) was not found.";
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

  return errCode;
}
