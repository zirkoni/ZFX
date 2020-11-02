#version 330

in vec3 positionIn;
in vec4 colourIn;

out vec4 colour;

void main()
{
	gl_Position = vec4(positionIn, 1.0);
	colour = colourIn;
}

