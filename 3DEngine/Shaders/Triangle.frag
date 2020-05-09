#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 position;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;
uniform sampler2D thisTexture;

void main() {
  // Ambient component
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse component
  vec3 lightDir = normalize(light.position - FragPos);
  vec3 norm = normalize(Normal);
  float diffuseFactor = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * material.diffuse * diffuseFactor;

  // Specular component
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * material.specular * specularFactor;

  // Calculate resultant output color
  vec3 color = ambient + diffuse + specular;
  FragColor = texture(thisTexture, TexCoord) * vec4(color, 1.0f);
}