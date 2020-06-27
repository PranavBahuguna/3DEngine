#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec4 position;
  vec3 coneDir;
  float innerConeAngle;
  float outerConeAngle;
  float constant;
  float linear;
  float quadratic;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;
uniform sampler2D thisTexture;

void main() {
  vec3 lightDir;
  float attenuation = 1.0;

  if (light.position.w == 0.0f) {
    // Directional light - no attenuation changes
    lightDir = normalize(light.position.xyz);
  } else {
    // Point light - attenuation affected by distance
    vec3 distVec = light.position.xyz - FragPos;
    float dist = length(distVec);
    lightDir = normalize(distVec);
    attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    if (light.innerConeAngle > 0.0f) {
      // Spot light - adjust attenuation for light cone
      float theta = dot(-lightDir, normalize(light.coneDir));
      float epsilon = light.innerConeAngle - light.outerConeAngle;
      float intensity = clamp((theta - light.outerConeAngle) / epsilon, 0.0, 1.0);
      attenuation *= intensity;
    }
  }

  // Ambient component
  vec3 ambient = light.ambient * material.ambient * attenuation;

  // Diffuse component
  vec3 norm = normalize(Normal);
  float diffuseFactor = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * material.diffuse * diffuseFactor * attenuation;

  // Specular component
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * material.specular * specularFactor * attenuation;

  // Calculate resultant output color
  vec3 color = ambient + diffuse + specular;
  FragColor = texture(thisTexture, TexCoord) * vec4(color, 1.0f);
}