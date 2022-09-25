#version 330

in vec3 v_out_position;
in vec3 v_out_normal;
in vec2 v_out_texCoord;

out vec4 f_out_colour;

struct Material
{
    float shininess;
    sampler2D diffuse;
    sampler2D specular;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    float constant;
    float linear;
    float quadratic;

    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;

    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform vec3 viewPosition;
uniform Material material;

#define NUM_POINT_LIGHTS 4
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 difSample, vec3 specSample)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine
    vec3 ambient = light.ambient * difSample;
    vec3 diffuse = light.diffuse * diff * difSample;
    vec3 specular = light.specular * spec * specSample;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 difSample, vec3 specSample)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.ambient * difSample;
    vec3 diffuse = light.diffuse * diff * difSample;
    vec3 specular = light.specular * spec * specSample;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 difSample, vec3 specSample)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    intensity *= attenuation;

    // Combine
    vec3 ambient = light.ambient * difSample;
    vec3 diffuse = light.diffuse * diff * difSample;
    vec3 specular = light.specular * spec * specSample;
    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(v_out_normal);
    vec3 viewDir = normalize(viewPosition - v_out_position);

    // Texture sampling
    vec3 difSample = vec3(texture(material.diffuse, v_out_texCoord));
    vec3 specSample = vec3(texture(material.specular, v_out_texCoord));

    // Calculate different lighting components
    vec3 result = calcDirLight(dirLight, norm, viewDir, difSample, specSample);

    for(int i = 0; i < NUM_POINT_LIGHTS; ++i)
    {
        result += calcPointLight(pointLights[i], norm, v_out_position, viewDir, difSample, specSample);
    }

    result += calcSpotLight(spotLight, norm, v_out_position, viewDir, difSample, specSample);
    f_out_colour = vec4(result, 1.0);
}
