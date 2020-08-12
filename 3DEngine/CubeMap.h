#pragma once

#include "Texture.h"

#include <vector>

class CubeMap;
using CbmSptr = std::shared_ptr<CubeMap>;

class CubeMap : public Resource {
public:
  CubeMap(const std::string &folder, const std::string &extension);
  CubeMap(const std::string &name, const std::vector<std::string> &files);
  ~CubeMap();

  void use() const;

private:
  ERROR load(const std::string &filepath, GLuint faceIndex);

  GLuint m_textureID;
};
