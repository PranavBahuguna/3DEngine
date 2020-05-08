#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight {
  vec3 color;
  float ambientIntensity;
  vec3 pos;
  float diffuseIntensity;
};

struct Material {
  float specularIntensity;
  float shine;
};

uniform sampler2D thisTexture;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 viewPos;

void main() {
  vec3 ambient = directionalLight.color * directionalLight.ambientIntensity;

  vec3 lightDir = normalize(directionalLight.pos - FragPos);
  vec3 norm = normalize(Normal);
  float diffuseFactor = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = directionalLight.color * directionalLight.diffuseIntensity * diffuseFactor;
      
  vec3 specular = vec3(0, 0, 0);
  if (diffuseFactor > 0.0f) {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularFactor = dot(viewDir, reflectDir);
    if (specularFactor > 0.0f) {
      specularFactor = pow(specularFactor, material.shine);
      specular = directionalLight.color * material.specularIntensity * specularFactor;
    }    
  }

  color = texture(thisTexture, TexCoord) * vec4(ambient + diffuse + specular, 1.0f);
}