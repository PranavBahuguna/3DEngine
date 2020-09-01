#pragma once

#include <GL/glew.h>

class RenderBuffer {
public:
  RenderBuffer();
  ~RenderBuffer();

  void bind();
  void unbind();
  void setStorage(GLuint component, int w, int h);

  GLuint getId() const { return m_id; }

private:
  GLuint m_id;
};