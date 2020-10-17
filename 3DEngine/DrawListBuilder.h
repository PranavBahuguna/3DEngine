#pragma once

#include "BasicDrawList.h"
#include "DrawListDecorator.h"
#include "Game.h"
#include "IlluminationDrawList.h"
#include "ShadowMappingDrawList.h"
#include "TransparencyDrawList.h"

#include <glm/gtc/matrix_transform.hpp>

class DrawListBuilder {
public:
  // Basic draw list sets targets and shader
  static DrawListUptr CreateDrawList(ShaderSptr shader) {
    DrawListUptr basicDrawList = DrawListUptr(new BasicDrawList());
    basicDrawList->setShader(shader);

    return basicDrawList;
  }

  // Sets the max number of lights and number of available lights for the shader
  static DrawListUptr AddIllumination(DrawListUptr drawList, const std::vector<LightSptr> &lights) {
    ShaderSptr shader = drawList->getShader();
    shader->use();

    shader->setInt("nLights", static_cast<int>(lights.size()));
    shader->setInt("thisTexture", 0);
    shader->setInt("shadowMap", 1);

    return DrawListUptr(new IlluminationDrawList(std::move(drawList), lights));
  }

  // Apply shadow mapping
  static DrawListUptr AddShadowMapping(DrawListUptr drawList,
                                       const std::vector<LightSptr> &lights) {
    return DrawListUptr(new ShadowMappingDrawList(std::move(drawList), lights));
  }

  // Allows sorting of targets by distance from camera enabled for the drawlist
  static DrawListUptr AddTransparency(DrawListUptr drawList) {
    return DrawListUptr(new TransparencyDrawList(std::move(drawList)));
  }

  // Applies orthographic projection for all drawlist targets
  static DrawListUptr AddOrthoProjection(DrawListUptr drawList) {
    ShaderSptr shader = drawList->getShader();
    shader->use();

    shader->setMat4("projection", glm::ortho(0.0f, (float)Game::GetWindow().getWidth(), 0.0f,
                                             (float)Game::GetWindow().getHeight(), 0.0f, 1.0f));
    return drawList;
  }

private:
  DrawListBuilder() {} // prevent construction of this class
};