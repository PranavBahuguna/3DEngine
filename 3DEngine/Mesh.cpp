#include "Mesh.h"
#include "Resources.h"

// Constructor
Mesh::Mesh(const aiMesh *aiMesh) {
  std::vector<GLfloat> verts;
  std::vector<GLfloat> uvs;
  std::vector<GLfloat> normals;

  generate(aiMesh, verts, uvs, normals);

  m_numVerts = (GLsizei)verts.size();

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // Bind vertex and texture coords data
  glGenBuffers(NUM_VBO, m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(verts[0]), verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(uvs[0]), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  //m_material = Resources::GetMaterial(mat);
}

// Destructor
Mesh::~Mesh() {
  glDeleteBuffers(NUM_VBO, m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

// Generate the mesh from the assimp mesh object
void Mesh::generate(const aiMesh *mesh, std::vector<GLfloat> &verts, std::vector<GLfloat> &uvs,
                    std::vector<GLfloat> &normals) const {

  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    // Vertices
    verts.push_back(mesh->mVertices[i].x);
    verts.push_back(mesh->mVertices[i].y);
    verts.push_back(mesh->mVertices[i].z);

    // Texture coordinates
    if (mesh->mTextureCoords[0]) {
      uvs.push_back(mesh->mTextureCoords[0][i].x);
      uvs.push_back(mesh->mTextureCoords[0][i].y);
    } else {
      uvs.insert(uvs.end(), {0.0f, 0.0f});
    }

    // Normals
    normals.push_back(-mesh->mNormals[i].x);
    normals.push_back(-mesh->mNormals[i].y);
    normals.push_back(-mesh->mNormals[i].z);
  }
}

// Draws the mesh onto the screen
void Mesh::draw() const {
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
}
