#version 330

layout (location = 0) in vec2 v_in_position;
layout (location = 1) in vec2 v_in_texCoord;

out vec2 v_out_texCoord;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_model * vec4(v_in_position, 0.0, 1.0);
    v_out_texCoord = v_in_texCoord;
}

