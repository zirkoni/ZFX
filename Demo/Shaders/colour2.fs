#version 330
out vec4 f_out_colour;
uniform vec4 u_colour;

void main()
{
    f_out_colour = u_colour;
}