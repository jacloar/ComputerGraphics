#version 330

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 fragmentColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

struct PointLight {
    vec3 color;
    vec3 position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform PointLight pointLights[1];  


void main() {

  vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
  normal = normalize(normal * 2.0 - 1.0);

  vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

  //ambient
  vec3 ambient = pointLights[0].ambientIntensity * pointLights[0].color;

  // Diffuse
  vec3 lightDir = normalize(pointLights[0].position - fs_in.FragPos);
  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * pointLights[0].color;

  //specuar
  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

  vec3 specular = pointLights[0].specularIntensity * spec * pointLights[0].color;

  vec3 Lighting = diffuseLight + ambient + specular;
  
  fragmentColor = vec4(color * Lighting,1.0);
 }
