#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec4 colourIn;
layout (location = 2) in vec2 texCoordIn;

out vec4 colour;
uniform mat4 transform;
varying vec2 texCoord;

void main()
{
	gl_Position = transform * vec4(positionIn, 1.0);
	colour = colourIn;
    texCoord = texCoordIn;
}

