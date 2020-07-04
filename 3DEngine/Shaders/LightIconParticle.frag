#version 330 core

#define TOLERANCE 0.1

in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

void main() {
  if (color.a < TOLERANCE)
    discard;

  color = color = texture(sprite, TexCoords);
  if (color.a < 1.0 - TOLERANCE)
    color *= ParticleColor;
}