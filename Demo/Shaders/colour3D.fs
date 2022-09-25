#version 330

out vec4 f_out_colour;
uniform vec4 colour = vec4(0.5, 0.5, 0.5, 1.0); // default colour, grey

void main()
{
    f_out_colour = colour;
}