#include "RenderBuffer.h"

RenderBuffer::RenderBuffer() { glGenRenderbuffers(1, &m_id); }

RenderBuffer::~RenderBuffer() { glDeleteRenderbuffers(1, &m_id); }

void RenderBuffer::bind() { glBindRenderbuffer(GL_RENDERBUFFER, m_id); }

void RenderBuffer::unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void RenderBuffer::setStorage(GLuint component, int w, int h) {
  glRenderbufferStorage(GL_RENDERBUFFER, component, w, h);
}