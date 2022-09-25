#version 330
in vec2 v_out_texCoord;
out vec4 f_out_colour;
uniform sampler2D diffuse;

void main()
{
    f_out_colour = texture(diffuse, v_out_texCoord);
}