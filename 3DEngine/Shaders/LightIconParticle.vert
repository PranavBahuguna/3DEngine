#version 330 core

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;
uniform float scale = 1.0;

void main() {
  TexCoords = vertex.zw;
  ParticleColor = color;

  // Remove rotation component from modelview matrix
  mat4 modelView = view * model;
  mat4 bbModelView = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0),
                          vec4(0.0, 0.0, 1.0, 0.0), modelView[3]);

  mat4 mvp = projection * bbModelView;
  gl_Position = mvp * vec4(vertex.xy * scale, 1.0, 1.0);
}