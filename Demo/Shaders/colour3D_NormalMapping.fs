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
    sampler2D normal;
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

uniform vec3 u_viewPosition;
uniform Material u_material;

#define NUM_POINT_LIGHTS 2
uniform PointLight u_pointLights[NUM_POINT_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 difSample, vec3 specSample)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

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

void main()
{
    // Flip V coordinate for proper texture mapping
    vec2 texCoord = vec2(v_out_texCoord.x, 1.0 - v_out_texCoord.y);
    
    // Sample textures
    vec3 difSample = vec3(texture(u_material.diffuse, texCoord));
    vec3 specSample = vec3(texture(u_material.specular, texCoord));
    
    // Use interpolated normals
    vec3 norm = normalize(v_out_normal);
    
    vec3 viewDir = normalize(u_viewPosition - v_out_position);

    // Calculate point lighting
    vec3 result = vec3(0.0);
    for(int i = 0; i < NUM_POINT_LIGHTS; ++i)
    {
        result += calcPointLight(u_pointLights[i], norm, v_out_position, viewDir, difSample, specSample);
    }

    f_out_colour = vec4(result, 1.0);
}

