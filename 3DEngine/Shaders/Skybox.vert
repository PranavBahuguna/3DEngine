#version 330

layout(location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
  TexCoords = pos;
  vec4 wvp = projection * view * vec4(pos, 1.0);
  gl_Position = wvp.xyww;
}