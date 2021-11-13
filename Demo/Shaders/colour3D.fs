#version 330

out vec4 fragColorOut;
uniform vec4 colour = vec4(0.5, 0.5, 0.5, 1.0); // default colour, grey

void main()
{
	fragColorOut = colour;
}