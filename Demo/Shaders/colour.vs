#version 330

layout (location = 0) in vec2 v_in_position;
layout (location = 1) in vec4 v_in_colour;

out vec4 v_out_colour;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_model * vec4(v_in_position, 0.0, 1.0);
    v_out_colour = v_in_colour;
}

