#version 330 core

layout (location = 0) in vec3 v_in_position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(v_in_position, 1.0);
}
