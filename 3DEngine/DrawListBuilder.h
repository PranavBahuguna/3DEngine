#pragma once

#include "IlluminationDrawList.h"
#include "TransparencyDrawList.h"

#include <glm/gtc/matrix_transform.hpp>

class DrawListBuilder {
public:
  // Basic draw list sets targets and shader
  static DrawListUptr CreateDrawList(DrawTargets targets, const std::string &shader) {
    DrawListUptr basicDrawList = DrawListUptr(new BasicDrawList());
    basicDrawList->setTargets(std::move(targets));
    basicDrawList->setShader(shader);

    return basicDrawList;
  }

  // Sets the max number of lights and number of available lights for the shader
  static DrawListUptr AddIllumination(DrawListUptr drawList, const std::vector<LightSptr> &lights) {
    const auto ilDrawList = new IlluminationDrawList(std::move(drawList), lights);

    ShaderSptr _shader = ilDrawList->getShader();
    _shader->setInt("nLights", static_cast<int>(lights.size()));

    return DrawListUptr(ilDrawList);
  }

  // Allows sorting of targets by distance from camera enabled for the drawlist
  static DrawListUptr AddTransparency(DrawListUptr drawList) {
    return DrawListUptr(new TransparencyDrawList(std::move(drawList)));
  }

  // Applies orthographic projection for all drawlist targets
  static DrawListUptr AddOrthoProjection(DrawListUptr drawList) {
    ShaderSptr _shader = drawList->getShader();
    _shader->setMat4("projection", glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f,
                                              (float)Window::GetHeight(), 0.0f, 1.0f));
    return drawList;
  }

private:
  DrawListBuilder() {} // prevent construction of this class
};