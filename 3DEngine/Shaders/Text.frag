#version 330

in vec2 TexCoords;

out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {
  color = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r) * textColor;
}