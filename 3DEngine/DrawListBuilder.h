#pragma once

#include "IlluminationDrawList.h"
#include "TransparencyDrawList.h"

class DrawListBuilder {
public:
  static DListPtr CreateDrawList(DrawTargets targets, const std::string &shader) {
    DListPtr basicDrawList = DListPtr(new BasicDrawList());
    basicDrawList->setTargets(std::move(targets));
    basicDrawList->setShader(shader);

    return basicDrawList;
  }

  static DListPtr AddIllumination(DListPtr drawList, const std::vector<std::string> &lightNames) {
    const auto ilDrawList = new IlluminationDrawList(std::move(drawList));
    ilDrawList->setLights(lightNames);

    ERROR errCode = ERROR_OK;
    ShaderPtr _shader = ilDrawList->getShader();
    if (!_shader->isCompiled()) {
      _shader->setPreprocessor(GL_FRAGMENT_SHADER, "MAX_LIGHTS", 8);
      _shader->compile(errCode);
    }
    _shader->setInt("nLights", static_cast<int>(lightNames.size()), errCode);

    return DListPtr(ilDrawList);
  }

  static DListPtr AddTransparency(DListPtr drawList) {
    return DListPtr(new TransparencyDrawList(std::move(drawList)));
  }

private:
  DrawListBuilder() {} // prevent construction of this class
};