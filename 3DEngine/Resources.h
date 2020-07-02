#pragma once

#include "Font.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

template <typename T> using ResourceMap = std::unordered_map<std::string, std::weak_ptr<T>>;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Material> MatPtr;
typedef std::shared_ptr<Light> LightPtr;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<Texture> TexPtr;
typedef std::shared_ptr<Font> FontPtr;

static ResourceMap<Mesh> meshMap;
static ResourceMap<Material> materialMap;
static ResourceMap<Light> lightMap;
static ResourceMap<Shader> shaderMap;
static ResourceMap<Texture> textureMap;
static ResourceMap<Font> fontMap;

class Resources {
public:
  static MeshPtr FindMesh(const std::string &name);
  static MeshPtr CreateMesh(const std::string &name, const std::vector<GLfloat> &vertices,
                            const std::vector<GLfloat> &texCoords,
                            const std::vector<GLfloat> &normals,
                            const std::vector<GLuint> &indices);
  static MatPtr FindMaterial(const std::string &name);
  static MatPtr CreateMaterial(const std::string &name, const glm::vec3 &ambient,
                               const glm::vec3 &diffuse, const glm::vec3 &specular,
                               GLfloat shininess);
  static LightPtr FindLight(const std::string &name);
  static LightPtr CreateDirectionalLight(const std::string &name, const glm::vec3 &ambient,
                                         const glm::vec3 &diffuse, const glm::vec3 &specular,
                                         const glm::vec3 &direction);
  static LightPtr CreatePointLight(const std::string &name, const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse, const glm::vec3 &specular,
                                   const glm::vec3 &position, GLfloat constant, GLfloat linear,
                                   GLfloat quadratic);
  static LightPtr CreateSpotLight(const std::string &name, const glm::vec3 &ambient,
                                  const glm::vec3 &diffuse, const glm::vec3 &specular,
                                  const glm::vec3 &position, GLfloat constant, GLfloat linear,
                                  GLfloat quadratic, const glm::vec3 &coneDir,
                                  GLfloat innerConeAngle, GLfloat outerConeAngle);
  static ShaderPtr GetShader(const std::string &name);
  static TexPtr GetTexture(const std::string &name);
  static FontPtr GetFont(const std::string &name);

private:
};
