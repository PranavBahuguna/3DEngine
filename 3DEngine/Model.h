#pragma once

#include "AssetLoader.h"
#include "Component.h"
#include "Light.h"
#include "Shader.h"

class Model : public Component {
public:
  Model(const std::string &name);
  Model(std::shared_ptr<GameObject> owner, const std::string &name);

  virtual void load(ERROR &errCode);

  void draw(ERROR &errCode, const Shader &shader) override;

protected:
  std::string m_name;
  AssetPtrs<Mesh> m_meshes;
  AssetPtrs<Texture> m_textures;
  AssetPtrs<Material> m_materials;

  TexSptr m_depthTexture;
};