#version 330

layout (location = 0) in vec3 v_in_position;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_model * vec4(v_in_position, 1.0);
}

