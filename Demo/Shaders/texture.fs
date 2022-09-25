#version 330
in vec2 v_out_texCoord;
out vec4 f_out_colour;
uniform sampler2D u_diffuse;

void main()
{
    f_out_colour = texture(u_diffuse, v_out_texCoord);
}