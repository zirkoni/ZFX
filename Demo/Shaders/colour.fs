#version 330

in vec4 v_out_colour; // Input from vertex shader, same type and name

out vec4 f_out_colour;

void main()
{
    f_out_colour = v_out_colour;
}