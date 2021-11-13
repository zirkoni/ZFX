#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;

out vec3 position;
out vec3 normal;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(positionIn, 1.0);
    normal = (transform * vec4(normalIn, 0.0)).xyz;
    position = gl_Position.xyz;
}

