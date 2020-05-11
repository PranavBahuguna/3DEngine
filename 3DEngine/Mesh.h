#pragma once

#include "Error.h"
#include "Material.h"

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <assimp/scene.h>

class Mesh {
public:
  Mesh(const aiMesh &mesh);
  ~Mesh();

  void draw() const;

private:
  static const int NUM_VBO = 3;
  static const GLuint VERTEX_STRIDE = 5;

  void create(const aiMesh &mesh, std::vector<float> &vertices, std::vector<float> &uvs,
              std::vector<float> &normals, std::vector<GLuint> &indices) const;

  GLuint m_IBO;
  GLuint m_VAO;
  GLuint m_VBO[NUM_VBO];
  GLsizei m_numIndices;
};
