#pragma once

#include "Error.h"
#include "Material.h"
#include "tiny_obj_loader.h"

#include <vector>
#include <memory>

#include <GL/glew.h>

class Mesh {
public:
  Mesh(const std::string &name);
  ~Mesh();

  void draw() const;
  std::shared_ptr<Material> getMaterial() const { return m_material; }

private:
  static const int NUM_VBO = 3;
  static const GLuint VERTEX_STRIDE = 5;

  ERROR load(const std::string &meshFile, const std::string &matDir, std::vector<GLfloat> &vertices,
             std::vector<GLfloat> &uvs, std::vector<GLfloat> &normals,
             tinyobj::material_t &mat) const;

  std::shared_ptr<Material> m_material;

  GLuint m_VAO;
  GLuint m_VBO[NUM_VBO];
  GLsizei m_numVerts;
};
