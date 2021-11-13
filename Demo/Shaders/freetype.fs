#version 330 core
in vec2 texCoord;
out vec4 fragColourOut;

uniform sampler2D diffuse;
uniform vec3 textColour;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(diffuse, texCoord).r);
    fragColourOut = vec4(textColour, 1.0) * sampled;
}
