#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 model;

void main() {
  gl_Position = mvp * vec4(pos, 1.0);

  TexCoord = tex;

  Normal = mat3(model) * norm;
  FragPos = vec3(model * vec4(pos, 1.0));
}