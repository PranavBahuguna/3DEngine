#version 330

#define MAX_LIGHTS 8

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
  vec3 color;
  vec4 position;
  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
  float constant;
  float linear;
  float quadratic;
  vec3 coneDir;
  float innerConeAngle;
  float outerConeAngle;
};

uniform vec3 viewPos;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform sampler2D thisTexture;
uniform int nLights;

// forward method declarations
vec3 calcLight(Light light, Material mat, vec3 viewDir, vec3 norm, vec3 fragPos);

void main() {
  vec3 resultColor = vec3(0.0f);
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  for (int i = 0; i < min(nLights, MAX_LIGHTS); i++)
    resultColor += calcLight(lights[i], material, viewDir, norm, FragPos);

  FragColor = texture(thisTexture, TexCoord) * vec4(resultColor, 1.0f);
}

vec3 calcLight(Light light, Material mat, vec3 viewDir, vec3 norm, vec3 fragPos) {
  vec3 lightDir;
  float attenuation = 1.0;

  if (light.position.w == 0.0f) {
    // Directional light - no attenuation changes
    lightDir = normalize(light.position.xyz);
  } else {
    // Point light - attenuation affected by distance
    vec3 distVec = light.position.xyz - fragPos;
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
  vec3 ambient = light.ambientStrength * mat.ambient;

  // Diffuse component
  float diffuseFactor = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuseStrength * mat.diffuse * diffuseFactor;

  // Specular component
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), mat.shininess);
  vec3 specular = light.specularStrength * mat.specular * specularFactor;

  // Calculate resultant output color
  return (ambient + diffuse + specular) * light.color * attenuation;
}