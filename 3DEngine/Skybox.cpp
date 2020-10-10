#include "Skybox.h"
#include "Game.h"
#include "ResourceManager.h"

#include <stdexcept>

// Skybox cube vertices
static constexpr float vertices[] = {
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, // T1
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, // T2
    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, // T3
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  // T4
    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  // T5
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, // T6
    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  // T7
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  // T8
    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  // T9
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, // T10
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, // T11
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f   // T12
};

Skybox::Skybox(const std::string &cubeMapFolder, const std::vector<std::string> &files)
    : Drawable{glm::vec3(0.0f)}, m_cubeMap(ResourceManager<CubeMap>::Get(cubeMapFolder, files)) {

  // Configure VAO / VBO for skybox and bind buffer data
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  }
}

// Draws the skybox on screen
void Skybox::draw(ERROR &errCode, const Shader &shader) const {
  // Store old depth func mode and change to GL_LEQUAL - this ensures depth test passes when
  // values are equal to depth buffer content
  GLint oldDepthFuncMode;
  glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
  glDepthFunc(GL_LEQUAL);

  // Set shader parameters and apply
  shader.setMat4("view", glm::mat4(glm::mat3(Game::GetCamera().getView()))); // remove translation
  shader.setMat4("projection", Game::GetCamera().getProjection());

  // Set cubemap texture
  m_cubeMap->use();

  // Draw triangle array mesh
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthFunc(oldDepthFuncMode); // Reset depth function to old mode
}
