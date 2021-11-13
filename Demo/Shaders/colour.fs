#version 330

in vec4 colour; // Input from vertex shader, same type and name (basicshader.vs: out vec4 colour).

out vec4 fragColorOut;

void main()
{
	fragColorOut = colour;
}