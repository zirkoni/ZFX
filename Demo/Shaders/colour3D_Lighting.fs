#version 330

in vec3 position;
in vec3 normal;

out vec4 fragColorOut;

uniform vec4 colour = vec4(0.5, 0.5, 0.5, 1.0); // default colour, grey
uniform vec3 lightPosition = vec3(1.2, 1.0, 2.0);
uniform vec3 lightColour = vec3(1.0, 1.0, 1.0); // white

void main()
{
    // Ambient
    float ambientLightStrength = 0.5;
    vec3 ambient = ambientLightStrength * lightColour;
    
    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - position);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColour;
    
    vec3 result = (ambient + diffuse) * colour;
    fragColorOut = vec4(result, 1.0);
}