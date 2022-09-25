#version 330

layout (location = 0) in vec3 v_in_position;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(v_in_position, 1.0);
}

