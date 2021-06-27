#version 330

layout (location = 0) in vec3 positionIn;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(positionIn, 1.0);
}

