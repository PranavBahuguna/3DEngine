#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  FragPos = vec3(model * vec4(pos, 1.0));
  TexCoord = tex;
  Normal = mat3(transpose(inverse(model))) * norm;

  gl_Position = projection * view * vec4(FragPos, 1.0);
}