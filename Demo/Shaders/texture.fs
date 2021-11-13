#version 330

out vec4 fragColorOut;
uniform sampler2D diffuse;
varying vec2 texCoord;

void main()
{
    fragColorOut = texture(diffuse, texCoord);
}