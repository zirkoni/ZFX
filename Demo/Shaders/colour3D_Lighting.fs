#version 330

in vec3 v_out_position;
in vec3 v_out_normal;

out vec4 f_out_colour;

uniform vec4 u_colour = vec4(0.5, 0.5, 0.5, 1.0); // default colour, grey
uniform vec3 u_lightPosition = vec3(1.2, 1.0, 2.0);
uniform vec3 u_lightColour = vec3(1.0, 1.0, 1.0); // white
uniform vec3 u_viewPosition;

void main()
{
    // Ambient
    float ambientLightStrength = 0.1;
    vec3 ambient = ambientLightStrength * u_lightColour;

    // Diffuse
    vec3 norm = normalize(v_out_normal);
    vec3 lightDirection = normalize(u_lightPosition - v_out_position);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_lightColour;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPosition - v_out_position);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColour;

    vec3 result = (ambient + diffuse + specular) * u_colour.xyz;
    f_out_colour = vec4(result, 1.0);
}
