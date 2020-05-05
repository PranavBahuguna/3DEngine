#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight {
  vec3 color;
  vec3 direction;
  float ambientIntensity;
  float diffuseIntensity;
};

uniform sampler2D thisTexture;
uniform DirectionalLight directionalLight;

void main() {
  vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

  vec3 lightDir = normalize(directionalLight.direction - FragPos);
  float diffuseFactor = max(dot(normalize(Normal), lightDir), 0.0f);
  vec4 diffuseColor =
      vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

  color = texture(thisTexture, TexCoord) * (ambientColor + diffuseColor);
}