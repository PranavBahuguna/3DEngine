#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out VS_OUT {
  vec3 FragPos;
  vec2 TexCoords;
  vec3 Normal;
  vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
  vs_out.FragPos = vec3(model * vec4(pos, 1.0));
  vs_out.TexCoords = tex;
  vs_out.Normal = mat3(transpose(inverse(model))) * norm;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

  gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}