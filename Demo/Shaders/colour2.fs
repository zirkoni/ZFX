#version 330
out vec4 f_out_colour;
uniform vec4 colour;

void main()
{
    f_out_colour = colour;
}