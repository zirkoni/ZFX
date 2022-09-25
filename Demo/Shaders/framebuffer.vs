#version 330

layout (location = 0) in vec2 v_in_position;
layout (location = 1) in vec2 v_in_texCoord;

out vec2 v_out_texCoord;

void main()
{
    v_out_texCoord = v_in_texCoord;
    gl_Position = vec4(v_in_position, 0.0, 1.0);
}

