#version 450

out vec4 FragColor;

in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

in vec3 curPos;
in vec3 normal;


const int MAX_POINT_LIGHTS = 20;
const int MAX_DIRECTIONAL_LIGHTS = 20;

struct PointLight {
    vec4 color;
    vec3 position;
    float radius;
};

struct DirectionalLight {
    vec4 color;
    vec3 direction;
};

layout(binding = 0) uniform PointLightsU {
    PointLight pLights[MAX_POINT_LIGHTS];
};

layout(binding = 1) uniform DirectionalLightsU {
    DirectionalLight dLights[MAX_DIRECTIONAL_LIGHTS];
};

uniform uint point_light_count;
uniform uint directional_light_count;
uniform float directionalLightSpecularIntensity;
uniform vec3 cameraPos;

// Gets the Texture Unit from the main function
uniform sampler2D texture;


// Separate function for calculating point light contribution
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir) {

	vec3 lightDir = normalize(light.position - curPos);
    float dist = length(light.position - curPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));

    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Use the intensity stored in the 'w' component of the color vector
    float specularIntensity = light.color.w;
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32.0) * specularIntensity;

    // Calculate the combined lighting contribution
	return ((light.color.rgb * light.color.w * diffuse) + (light.color.rgb * specular)) * attenuation;
}

// Separate function for calculating directional light contribution
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularIntensity = light.color.w * directionalLightSpecularIntensity;
    float specular = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 8.0) * specularIntensity;

    // Calculate ambient lighting
    float ambientStrength = 0.2; // Adjust the ambient strength
    vec3 ambient = ambientStrength * light.color.rgb;

    // Calculate diffuse and specular lighting contributions
    vec3 diffuseLighting = light.color.rgb * light.color.w * diffuse;
    vec3 specularLighting = light.color.rgb * specular;

    // Combine the lighting components (diffuse + specular + ambient)
    vec3 lighting = diffuseLighting + specularLighting + ambient;

    return lighting;
} 


void main()
{
	vec3 viewDir = normalize(cameraPos - curPos);

    // Calculate total lighting contributions
    vec3 totalLighting = vec3(0.0);

    for (int i = 0; i < point_light_count; i++) {
        totalLighting += CalculatePointLight(pLights[i], normal, viewDir);
    }

    for (int i = 0; i < directional_light_count; i++) {
        totalLighting += CalculateDirectionalLight(dLights[i], normal, viewDir);
    }

    // Apply lighting to the final color
    FragColor = vec4(color, 1.0) * vec4(totalLighting, 1.0);
}