#include "FrameBuffer.h"
#include "Window.h"

FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &m_id); }

FrameBuffer::~FrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &m_id);
}

// Bind to this Framebuffer
void FrameBuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_id); }

// Binds to the default Framebuffer
void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

// Attach a texture to this Framebuffer
void FrameBuffer::attachTexture(GLuint textureId, FboAttachment attachment) {
  GLuint attachVal = static_cast<GLuint>(attachment);
  if (isColorAttachment(attachment)) {
    // Add attachment to color buffers if it does not already exist
    if (std::find(m_buffers.begin(), m_buffers.end(), attachVal) == m_buffers.end())
      m_buffers.push_back(attachVal);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachVal, GL_TEXTURE_2D, textureId, 0);
}

void FrameBuffer::setRenderBuffer(RenderBuffer &rBuf, FboAttachment attachment) {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLuint>(attachment), GL_RENDERBUFFER,
                            rBuf.getId());
}

// Draw all of the buffers
void FrameBuffer::drawBuffers() {
  glDrawBuffers(static_cast<int>(m_buffers.size()), &m_buffers.front());
}

// Checks if our Framebuffer is complete
bool FrameBuffer::isOk() const {
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}