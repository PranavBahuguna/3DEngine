#pragma once

#include "IlluminationDrawList.h"
#include "TransparencyDrawList.h"

#include <glm/gtc/matrix_transform.hpp>

class DrawListBuilder {
public:
  // Basic draw list sets targets and shader
  static DListPtr CreateDrawList(DrawTargets targets, const std::string &shader) {
    DListPtr basicDrawList = DListPtr(new BasicDrawList());
    basicDrawList->setTargets(std::move(targets));
    basicDrawList->setShader(shader);

    return basicDrawList;
  }

  // Sets the max number of lights and number of available lights for the shader
  static DListPtr AddIllumination(DListPtr drawList, const std::vector<std::string> &lightNames) {
    const auto ilDrawList = new IlluminationDrawList(std::move(drawList));
    ilDrawList->setLights(lightNames);

    ShaderPtr _shader = ilDrawList->getShader();
    _shader->setInt("nLights", static_cast<int>(lightNames.size()));

    return DListPtr(ilDrawList);
  }

  // Allows sorting of targets by distance from camera enabled for the drawlist
  static DListPtr AddTransparency(DListPtr drawList) {
    return DListPtr(new TransparencyDrawList(std::move(drawList)));
  }

  // Applies orthographic projection for all drawlist targets
  static DListPtr AddOrthoProjection(DListPtr drawList) {
    ShaderPtr _shader = drawList->getShader();
    _shader->setMat4("projection", glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f,
                                              (float)Window::GetHeight(), 0.0f, 1.0f));
    return drawList;
  }

private:
  DrawListBuilder() {} // prevent construction of this class
};