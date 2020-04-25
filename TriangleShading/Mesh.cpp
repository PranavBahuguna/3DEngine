#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.h"
#include "tiny_obj_loader.h"

// Constructor
Mesh::Mesh(const std::string &name) {

  std::vector<GLfloat> verts;
  std::vector<GLfloat> uvs;
  const std::string filepath = "Meshes/" + name + ".obj";

  if (load(filepath, verts, uvs) != ERROR_OK)
    throw std::runtime_error("An error occurred while loading mesh.");

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
}

// Destructor
Mesh::~Mesh() {
  glDeleteBuffers(NUM_VBO, m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

// Loads mesh from given file path
ERROR Mesh::load(const std::string &filepath, std::vector<GLfloat> &vertices,
                 std::vector<GLfloat> &uvs) const {
  ERROR errCode = ERROR_OK;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str());

  // Print any warning or error messages we get
  if (!warn.empty())
    printf(warn.c_str());
  if (!err.empty())
    printf(err.c_str());

  if (!ret) {
    errCode = ERROR_FILE_OPEN_FAILED;
    printErrorMsg(errCode, filepath);
  } else {
    // Loop over shapes
    for (auto &shape : shapes) {
      // Loop over faces
      size_t idxOffset = 0;
      for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
        int fv = shape.mesh.num_face_vertices[f];

        // Loop over face vertices
        for (size_t v = 0; v < fv; v++) {
          tinyobj::index_t idx = shape.mesh.indices[idxOffset + v];

          vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
          vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
          vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
          
          uvs.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
          uvs.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
        }
        idxOffset += fv;
      }
    }
  }

  return errCode;
}

// Draws the mesh onto the screen
void Mesh::draw() const {
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
}
