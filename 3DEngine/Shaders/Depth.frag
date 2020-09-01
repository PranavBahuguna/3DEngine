#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float nearPlane = 1.0f;
uniform float farPlane = 20.0f;
uniform bool isPerspective;

// To be used a perspective projection matrix is being applied
float LinearizeDepth(float depth) {
  float z = depth * 2.0 - 1.0; // back to NDC
  return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main() {
  float depthValue = texture(depthMap, TexCoords).r;
  if (isPerspective)
    FragColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0); // perspective
  else
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
