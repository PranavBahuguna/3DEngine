#version 330

#define MAX_LIGHTS 8

in VS_OUT {
  vec3 FragPos;
  vec2 TexCoords;
  vec3 Normal;
  vec4 FragPosLightSpace;
}
fs_in;

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
  bool isShadowCaster;
};

uniform vec3 viewPos;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform sampler2D thisTexture;
uniform sampler2D shadowMap;
uniform int nLights;

float calcShadow(vec4 fragPosLightSpace, float bias) {
  // Perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // Transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  // Get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // Check whether current frag pos is in shadow
  float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

  return shadow;
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

  // Shadow component
  float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
  float shadow = light.isShadowCaster ? calcShadow(fs_in.FragPosLightSpace, bias) : 0.0;

  // Calculate resultant output color
  return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color * attenuation;
}

void main() {
  vec3 resultColor = vec3(0.0f);
  vec3 norm = normalize(fs_in.Normal);
  vec3 viewDir = normalize(viewPos - fs_in.FragPos);

  // Calculate each light's contribution to the final result color
  for (int i = 0; i < min(nLights, MAX_LIGHTS); i++)
    resultColor += calcLight(lights[i], material, viewDir, norm, fs_in.FragPos);

  FragColor = texture(thisTexture, fs_in.TexCoords) * vec4(resultColor, 1.0f);
}