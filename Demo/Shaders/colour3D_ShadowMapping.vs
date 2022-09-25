#version 330 core

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texCoord;

out V_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLightSpace;
} v_out;

uniform mat4 u_viewProjection;
uniform mat4 u_model;
uniform mat4 u_lightSpaceMatrix;

void main()
{
    v_out.position = vec3(u_model * vec4(v_in_position, 1.0));
    v_out.normal = transpose(inverse(mat3(u_model))) * v_in_normal;
    v_out.texCoord = v_in_texCoord;
    v_out.fragPosLightSpace = u_lightSpaceMatrix * vec4(v_out.position, 1.0);
    gl_Position = u_viewProjection * u_model * vec4(v_in_position, 1.0);
}
