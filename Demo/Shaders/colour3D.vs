#version 330

layout (location = 0) in vec3 positionIn;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	gl_Position = viewProjection * model * vec4(positionIn, 1.0);
}

