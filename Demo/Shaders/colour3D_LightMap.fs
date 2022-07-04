#version 330

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColorOut;

struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient * texture(material.diffuse, texCoord).rgb;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(light.position - position);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

    // Specular
    vec3 viewDir = normalize(viewPosition - position);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

    vec3 result = (ambient + diffuse + specular);
    fragColorOut = vec4(result, 1.0);
}
