#pragma once

#include "Error.h"
#include "RenderBuffer.h"
#include "Texture.h"

#include <vector>

#include <GL/glew.h>

enum class FboAttachment {
  None = GL_NONE,
  Color0 = GL_COLOR_ATTACHMENT0,
  Color1 = GL_COLOR_ATTACHMENT1,
  Color2 = GL_COLOR_ATTACHMENT2,
  Color3 = GL_COLOR_ATTACHMENT3,
  Color4 = GL_COLOR_ATTACHMENT4,
  Color5 = GL_COLOR_ATTACHMENT5,
  Color6 = GL_COLOR_ATTACHMENT6,
  Color7 = GL_COLOR_ATTACHMENT7,
  Color8 = GL_COLOR_ATTACHMENT8,
  Color9 = GL_COLOR_ATTACHMENT9,
  Color10 = GL_COLOR_ATTACHMENT10,
  Color11 = GL_COLOR_ATTACHMENT11,
  Color12 = GL_COLOR_ATTACHMENT12,
  Color13 = GL_COLOR_ATTACHMENT13,
  Color14 = GL_COLOR_ATTACHMENT14,
  Color15 = GL_COLOR_ATTACHMENT15,
  Depth = GL_DEPTH_ATTACHMENT,
  Stencil = GL_STENCIL_ATTACHMENT,
  DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

class FrameBuffer {
public:
  FrameBuffer();
  ~FrameBuffer();

  void bind();
  void unbind();
  void attachTexture(GLuint textureId, FboAttachment attachment);
  void setRenderBuffer(RenderBuffer &rBuf, FboAttachment attachment);
  void drawBuffers();
  bool isOk() const;

  GLuint getId() const { return m_id; }

private:
  bool isColorAttachment(FboAttachment attachment) const {
    return attachment >= FboAttachment::Color0 && attachment <= FboAttachment::Color15;
  }

  GLuint m_id;
  std::vector<GLuint> m_buffers;
};