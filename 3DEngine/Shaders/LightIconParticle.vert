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
  mat4 modelView = view * model;
  
  modelView[0][0] = 1.0; 
  modelView[0][1] = 0.0; 
  modelView[0][2] = 0.0; 

  modelView[1][0] = 0.0; 
  modelView[1][1] = 1.0; 
  modelView[1][2] = 0.0;

  modelView[2][0] = 0.0; 
  modelView[2][1] = 0.0; 
  modelView[2][2] = 1.0; 

  vec4 P = modelView * vec4(vertex.xy * scale, 1.0, 1.0);
  gl_Position = projection * P;
}