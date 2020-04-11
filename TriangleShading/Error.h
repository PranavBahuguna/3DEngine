#pragma once

#include <cstdint>
#include <initializer_list>
#include <stdarg.h>
#include <string>
#include <unordered_map>

typedef uint32_t ERROR;

#define ERROR_OK                                0
#define ERROR_GLFW_INIT_FAILED                  1
#define ERROR_GLFW_WINDOW_CREATE_FAILED         2
#define ERROR_GLEW_INIT_FAILED                  3
#define ERROR_FILE_OPEN_FAILED                  4
#define ERROR_SHADER_COMPILE_FAILED             5
#define ERROR_SHADER_PROGRAM_CREATE_FAILED      6
#define ERROR_SHADER_PROGRAM_INVALID            7
#define ERROR_SHADER_PROGRAM_LINKING_FAILED     8

static void printErrorMsg(ERROR errCode, int nArgs = 0, ...) {
 
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
    errBody = "Unable to create shader program!";
    break;
  case ERROR_SHADER_PROGRAM_INVALID:
    errBody = "Error in validating program:\n%s";
    break;
  case ERROR_SHADER_PROGRAM_LINKING_FAILED:
    errBody = "Error in linking program:\n%s";
    break;
  default:
    errBody = "Unknown error";
    break;
  }

  std::string errMsg = errHeader + errBody + "\n";
  const char *fmt = errMsg.c_str();

  // Add all variadic arguments to be printed
  va_list args;
  va_start(args, nArgs);
  vfprintf(stderr, fmt, args);
  va_end(args);
}