#version 330

layout (location = 0) in vec2 positionIn;
layout (location = 1) in vec4 colourIn;

out vec4 colour;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(positionIn, 0.0, 1.0);
	colour = colourIn;
}

