#include "SpotLight.h"
#include "Projection.h"
#include "Timer.h"
#include "Transform.h"

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
  Transform lsTransform(m_position, glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
  Projection lsProjection(glm::radians(60.0f), 1.0f, 1.0f, 20.0f);

  m_lightSpaceMatrix = lsProjection.getProjection() * lsTransform.getView();
}