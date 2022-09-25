#version 330 core
layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texCoord;

out V_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} v_out;

uniform mat4 viewProjection;
uniform mat4 model;

uniform bool reverseNormals;

void main()
{
    v_out.position = vec3(model * vec4(v_in_position, 1.0));

    // A slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
    if(reverseNormals)
    {
        v_out.normal = transpose(inverse(mat3(model))) * (-1.0 * v_in_normal);
    } else
    {
        v_out.normal = transpose(inverse(mat3(model))) * v_in_normal;
    }

    v_out.texCoord = v_in_texCoord;
    gl_Position = viewProjection * model * vec4(v_in_position, 1.0);
}
