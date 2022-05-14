#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;

out vec3 position;
out vec3 normal;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    position = vec3(model * vec4(positionIn, 1.0));
    normal = mat3(transpose(inverse(model))) * normalIn;
    gl_Position = viewProjection * vec4(position, 1.0);
}

