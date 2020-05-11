#include "Mesh.h"
#include "Resources.h"

// Constructor
Mesh::Mesh(const aiMesh &aiMesh) {
  std::vector<float> verts;
  std::vector<float> uvs;
  std::vector<float> normals;
  std::vector<unsigned int> indices;

  create(aiMesh, verts, uvs, normals, indices);
  m_numIndices = (GLsizei)indices.size();

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind vertex, texture coords and normals data
  glGenBuffers(NUM_VBO, m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  // Bind index data buffer
  glGenBuffers(1, &m_IBO);
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);
  }
}

// Destructor
Mesh::~Mesh() {
  glDeleteBuffers(NUM_VBO, m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

// Generate the mesh from the assimp mesh object
void Mesh::create(const aiMesh &mesh, std::vector<GLfloat> &verts, std::vector<GLfloat> &uvs,
                  std::vector<GLfloat> &normals, std::vector<unsigned int> &indices) const {

  for (size_t i = 0; i < mesh.mNumVertices; i++) {
    // Vertices
    verts.push_back(mesh.mVertices[i].x);
    verts.push_back(mesh.mVertices[i].y);
    verts.push_back(mesh.mVertices[i].z);

    // Texture coordinates
    if (mesh.mTextureCoords[0]) {
      uvs.push_back(mesh.mTextureCoords[0][i].x);
      uvs.push_back(mesh.mTextureCoords[0][i].y);
    } else {
      uvs.insert(uvs.end(), {0.0f, 0.0f});
    }

    // Normals
    normals.push_back(mesh.mNormals[i].x);
    normals.push_back(mesh.mNormals[i].y);
    normals.push_back(mesh.mNormals[i].z);
  }

  // Find all mesh indices
  for (size_t i = 0; i < mesh.mNumFaces; i++) {
    aiFace face = mesh.mFaces[i];

    for (size_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
}

// Draws the mesh onto the screen
void Mesh::draw() const {
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
}
