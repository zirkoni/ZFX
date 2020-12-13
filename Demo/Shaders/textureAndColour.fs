#version 330
in vec4 colour; // Input from vertex shader

out vec4 fragColorOut;
uniform sampler2D diffuse;
varying vec2 texCoord;

void main()
{
    // Combine texture and colour
    fragColorOut = texture(diffuse, texCoord) * colour;
}