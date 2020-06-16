#include "Terrain.h"

// Constructor
Terrain::Terrain(const std::string &name, const glm::uvec2 &nTiles, const GLfloat tileSize)
    : Model(name), m_nTiles(nTiles), m_tileSize(tileSize) {}

// Generates the mesh and loads relevant materials
void Terrain::loadAssets(const aiScene &scene, ERROR &errCode) {
  generateMesh(errCode);
  loadMaterials(scene, errCode);
}

// Constructs model mesh
void Terrain::generateMesh(ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  size_t nVertices = ((size_t)m_nTiles.x + 1) * ((size_t)m_nTiles.y + 1);
  std::vector<GLfloat> vertices(nVertices * 3);
  std::vector<GLfloat> texCoords(nVertices * 2);
  std::vector<GLfloat> normals(nVertices * 3);
  std::vector<GLuint> indices(6 * (size_t)m_nTiles.x * m_nTiles.y);

  size_t vPtr = 0;
  for (GLuint i = 0; i < m_nTiles.y + 1; ++i) {
    for (GLuint j = 0; j < m_nTiles.x + 1; ++j) {
      vertices[vPtr * 3 + 0] = j * m_tileSize;
      vertices[vPtr * 3 + 1] = 0.0f;
      vertices[vPtr * 3 + 2] = i * m_tileSize;

      texCoords[vPtr * 2 + 0] = j % 2;
      texCoords[vPtr * 2 + 1] = i % 2;

      normals[vPtr * 3 + 0] = 0.0f;
      normals[vPtr * 3 + 1] = 1.0f;
      normals[vPtr * 3 + 2] = 0.0f;

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

  m_meshes.push_back(Resources::GetMesh(m_name, vertices, texCoords, normals, indices));
  m_meshToTex.push_back(0);
}