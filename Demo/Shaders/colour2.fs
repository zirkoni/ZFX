#version 330
out vec4 fragColorOut;
uniform vec4 colour;

void main()
{
	fragColorOut = colour;
}