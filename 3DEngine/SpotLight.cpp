#include "SpotLight.h"
#include "Timer.h"

#include <glm/gtc/matrix_transform.hpp>

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &color, float ambient,
                     float diffuse, float specular, float constant, float linear, float quadratic,
                     const glm::vec3 &coneDir, float innerConeAngle, float outerConeAngle,
                     bool isShadowCaster)
    : Light(position, false, color, ambient, diffuse, specular, isShadowCaster, constant, linear,
            quadratic, coneDir, innerConeAngle, outerConeAngle),
      m_lightSpaceMatrix(0.0f) {}

void SpotLight::use(const Shader &shader, const std::string &prefix) const {
  shader.setVec4(prefix + "position",
                 glm::vec4(m_position.x, m_position.y, m_position.z, m_positionW));
  shader.setVec3(prefix + "color", m_color);
  shader.setFloat(prefix + "ambientStrength", m_ambient);
  shader.setFloat(prefix + "diffuseStrength", m_diffuse);
  shader.setFloat(prefix + "specularStrength", m_specular);
  shader.setBool(prefix + "isShadowCaster", m_isShadowCaster);
  shader.setFloat(prefix + "constant", m_constant);
  shader.setFloat(prefix + "linear", m_linear);
  shader.setFloat(prefix + "quadratic", m_quadratic);
  shader.setVec3(prefix + "coneDir", m_coneDir);
  shader.setFloat(prefix + "innerConeAngle", m_innerConeAngle);
  shader.setFloat(prefix + "outerConeAngle", m_outerConeAngle);
}

void SpotLight::setLightSpaceMatrix(const Shader &shader) const {
  shader.setMat4("lightSpaceMatrix", m_lightSpaceMatrix);
}

void SpotLight::update(ERROR &errCode) {
  // Calculate the light space projection matrix
  float near_plane = 1.0f, far_plane = 20.0f;
  glm::mat4 lightSpaceProjection =
      glm::perspective(glm::radians(60.0f), 1.0f, near_plane, far_plane);

  // TODO: Implement the perspective projection class!
  /*
  // Calculate the right and up vectors using the light cone direction
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(m_coneDir, worldUp));
  glm::vec3 up = glm::normalize(glm::cross(right, m_coneDir));

  // Calculate the light space view, and the overall MVP light space matrix
  glm::mat4 lightSpaceView = glm::lookAt(m_position, m_position + m_coneDir, up);
  */
  glm::vec3 pos = {-4.0f, 10.0f, 3.0f};
  glm::vec3 tgt = {-4.0f, 0.0f, 3.0f};
  glm::vec3 up = {0.0f, 0.0f, 1.0f};
  glm::mat4 lightSpaceView = glm::lookAt(m_position, tgt, up);

  m_lightSpaceMatrix = lightSpaceProjection * lightSpaceView;
}