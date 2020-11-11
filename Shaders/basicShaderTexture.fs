#version 330

in vec4 colour; // Input from vertex shader, same type and name (basicshader.vs: out vec4 colour).

out vec4 fragColorOut;
uniform sampler2D diffuse;
varying vec2 texCoord;

void main()
{
    fragColorOut = texture2D(diffuse, texCoord) * colour;
}