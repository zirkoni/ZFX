#version 330

in vec2 texCoord;
out vec4 fragColorOut;

uniform sampler2D screenTexture;

void main()
{
    vec3 colour = texture(screenTexture, texCoord).rgb;
    fragColorOut = vec4(colour, 1.0);
}