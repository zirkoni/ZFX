#version 330
in vec4 v_out_colour; // Input from vertex shader
in vec2 v_out_texCoord;

out vec4 f_out_colour;
uniform sampler2D diffuse;

void main()
{
    // Combine texture and colour
    f_out_colour = texture(diffuse, v_out_texCoord) * v_out_colour;
}