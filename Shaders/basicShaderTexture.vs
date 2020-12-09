#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 texCoordIn;

uniform mat4 transform;
varying vec2 texCoord;

void main()
{
	gl_Position = transform * vec4(positionIn, 1.0);
    texCoord = texCoordIn;
}

