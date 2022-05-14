#version 330
layout (location = 0) in vec2 positionIn;

out vec2 position;
uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	gl_Position = viewProjection * model * vec4(positionIn, 0.0, 1.0);
    position = positionIn;
}

