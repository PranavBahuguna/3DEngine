#pragma once

#include "AssetLoader.h"
#include "Drawable.h"
#include "Light.h"
#include "Shader.h"

class Model : public Drawable {
public:
  Model(const std::string &name);

  virtual void load(ERROR &errCode);

  void draw(ERROR &errCode, const Shader &shader) const override;

protected:
  std::string m_name;
  AssetPtrs<Mesh> m_meshes;
  AssetPtrs<Texture> m_textures;
  AssetPtrs<Material> m_materials;

  TexSptr m_depthTexture;
};