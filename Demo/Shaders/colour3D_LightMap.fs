#version 330

in vec3 v_out_position;
in vec3 v_out_normal;
in vec2 v_out_texCoord;

out vec4 f_out_colour;

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

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform Light u_light;

void main()
{
    // Ambient
    vec3 ambient = u_light.ambient * u_material.ambient * texture(u_material.diffuse, v_out_texCoord).rgb;

    // Diffuse
    vec3 norm = normalize(v_out_normal);
    vec3 lightDirection = normalize(u_light.position - v_out_position);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = u_light.diffuse * diff * texture(u_material.diffuse, v_out_texCoord).rgb;

    // Specular
    vec3 viewDir = normalize(u_viewPosition - v_out_position);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * texture(u_material.specular, v_out_texCoord).rgb;

    vec3 result = (ambient + diffuse + specular);
    f_out_colour = vec4(result, 1.0);
}
