#version 330
layout (location = 0) in vec2 v_in_position;

out vec2 v_out_position;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(v_in_position, 0.0, 1.0);
    v_out_position = v_in_position;
}

