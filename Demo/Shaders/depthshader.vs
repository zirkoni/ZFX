#version 330 core

layout (location = 0) in vec3 positionIn;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(positionIn, 1.0);
}
