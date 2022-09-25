#version 330

in vec3 v_out_position;
in vec3 v_out_normal;

out vec4 f_out_colour;

uniform vec4 colour = vec4(0.5, 0.5, 0.5, 1.0); // default colour, grey
uniform vec3 lightPosition = vec3(1.2, 1.0, 2.0);
uniform vec3 lightColour = vec3(1.0, 1.0, 1.0); // white

void main()
{
    // Ambient
    float ambientLightStrength = 0.5;
    vec3 ambient = ambientLightStrength * lightColour;

    // Diffuse
    vec3 norm = normalize(v_out_normal);
    vec3 lightDirection = normalize(lightPosition - v_out_position);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColour;

    vec3 result = (ambient + diffuse) * colour.xyz;
    f_out_colour = vec4(result, 1.0);
}
