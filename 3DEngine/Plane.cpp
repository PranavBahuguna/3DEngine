#include "Plane.h"
#include "AssetLoader.h"

#include <fstream>

// Constructor
Plane::Plane(const std::string &name, const glm::uvec2 &nTiles, const glm::vec2 &tileDimensions,
             const glm::vec2 &tileTexMapping)
    : Model(name), m_nTiles(nTiles), m_tileDimensions(tileDimensions),
      m_tileTexMapping(tileTexMapping) {

  m_meshes.resize(1);
}

// Creates the mesh and relevant materials
void Plane::load(ERROR &errCode) {
  // Use existing mesh if available, otherwise generate the mesh
  if (!ResourceManager<Mesh>::Find(m_name, m_meshes[0]))
    generateMesh();

  AssetLoader::loadMaterials(errCode, m_name, m_materials, m_textures);
}

// Constructs plane mesh
void Plane::generateMesh() {
  size_t nVertices = ((size_t)m_nTiles.x + 1) * ((size_t)m_nTiles.y + 1);
  std::vector<GLfloat> vertices(nVertices * 3);
  std::vector<GLfloat> texCoords(nVertices * 2);
  std::vector<GLfloat> normals(nVertices * 3);
  std::vector<GLuint> indices(6 * (size_t)m_nTiles.x * m_nTiles.y);

  GLfloat startX = -(m_nTiles.x * m_tileDimensions.x / 2.0f);
  GLfloat startY = -(m_nTiles.y * m_tileDimensions.y / 2.0f);

  size_t vPtr = 0;
  for (GLuint i = 0; i < m_nTiles.y + 1; ++i) {
    for (GLuint j = 0; j < m_nTiles.x + 1; ++j) {
      vertices[vPtr * 3 + 0] = startX + j * m_tileDimensions.x;
      vertices[vPtr * 3 + 1] = 0.0f;
      vertices[vPtr * 3 + 2] = startY + i * m_tileDimensions.y;

      normals[vPtr * 3 + 0] = 0.0f;
      normals[vPtr * 3 + 1] = 1.0f;
      normals[vPtr * 3 + 2] = 0.0f;

      texCoords[vPtr * 2 + 0] = -(j * m_tileTexMapping.x);
      texCoords[vPtr * 2 + 1] = -(i * m_tileTexMapping.y);

      ++vPtr;
    }
  }

  size_t iPtr = 0;
  for (GLuint i = 0; i < m_nTiles.x; ++i) {
    for (GLuint j = 0; j < m_nTiles.y; ++j) {
      GLint topLeft = (i * (m_nTiles.x + 1)) + j;
      GLint topRight = topLeft + 1;
      GLint bottomLeft = (i + 1) * (m_nTiles.x + 1) + j;
      GLint bottomRight = bottomLeft + 1;

      indices[iPtr++] = topLeft;
      indices[iPtr++] = bottomLeft;
      indices[iPtr++] = topRight;
      indices[iPtr++] = topRight;
      indices[iPtr++] = bottomLeft;
      indices[iPtr++] = bottomRight;
    }
  }

  m_meshes[0] = ResourceManager<Mesh>::Get(m_name, vertices, texCoords, normals, indices);
}