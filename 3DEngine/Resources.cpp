#include "Resources.h"

MeshPtr Resources::FindMesh(const std::string &name) {
  auto &mesh = meshMap[name];
  return (mesh.expired()) ? mesh.lock() : nullptr;
}

MeshPtr Resources::CreateMesh(const std::string &name, const std::vector<float> &vertices,
                              const std::vector<float> &texCoords,
                              const std::vector<float> &normals,
                              const std::vector<GLuint> &indices) {
  auto &mesh = meshMap[name];
  MeshPtr newMesh(new Mesh(vertices, texCoords, normals, indices));
  mesh = newMesh;
  return newMesh;
}

MatPtr Resources::FindMaterial(const std::string &name) {
  auto &material = materialMap[name];
  return (material.expired()) ? nullptr : material.lock();
}

MatPtr Resources::CreateMaterial(const std::string &name, const glm::vec3 &ambient,
                                 const glm::vec3 &diffuse, const glm::vec3 &specular,
                                 float shininess) {
  auto &material = materialMap[name];
  MatPtr newMaterial(new Material(ambient, diffuse, specular, shininess));
  material = newMaterial;
  return newMaterial;
}

LightPtr Resources::FindLight(const std::string &name) {
  auto &light = lightMap[name];
  return (light.expired()) ? nullptr : light.lock();
}

LightPtr Resources::CreateDirectionalLight(const std::string &name, const glm::vec3 &direction,
                                           const glm::vec3 &color, float ambient, float diffuse,
                                           float specular) {
  auto &light = lightMap[name];
  LightPtr newLight(
      new Light(LightType::DIRECTIONAL_LIGHT, direction, color, ambient, diffuse, specular));
  light = newLight;
  return newLight;
}

LightPtr Resources::CreatePointLight(const std::string &name, const glm::vec3 &position,
                                     const glm::vec3 &color, float ambient, float diffuse,
                                     float specular, float constant, float linear,
                                     float quadratic) {
  auto &light = lightMap[name];
  LightPtr newLight(new Light(LightType::POINT_LIGHT, position, color, ambient, diffuse, specular,
                              constant, linear, quadratic));
  light = newLight;
  return newLight;
}

LightPtr Resources::CreateSpotLight(const std::string &name, const glm::vec3 &position,
                                    const glm::vec3 &color, float ambient, float diffuse,
                                    float specular, float constant, float linear, float quadratic,
                                    const glm::vec3 &coneDir, float innerConeAngle,
                                    float outerConeAngle) {
  auto &light = lightMap[name];
  LightPtr newLight(new Light(LightType::SPOT_LIGHT, position, color, ambient, diffuse, specular,
                              constant, linear, quadratic, coneDir, innerConeAngle,
                              outerConeAngle));
  light = newLight;
  return newLight;
}

ShaderPtr Resources::GetShader(const std::string &name) {
  auto &shader = shaderMap[name];
  if (shader.expired()) {
    ShaderPtr newShader(new Shader(name));
    shader = newShader;
    return newShader;
  } else {
    return shader.lock();
  }
}

TexPtr Resources::GetTexture(const std::string &name) {
  auto &texture = textureMap[name];
  if (texture.expired()) {
    TexPtr newTexture(new Texture(name));
    texture = newTexture;
    return newTexture;
  } else {
    return texture.lock();
  }
}

FontPtr Resources::GetFont(const std::string &name) {
  auto &font = fontMap[name];
  if (font.expired()) {
    FontPtr newFont(new Font(name));
    font = newFont;
    return newFont;
  } else {
    return font.lock();
  }
}