#version 330

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;

out vec3 v_out_position;
out vec3 v_out_normal;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

void main()
{
    v_out_position = vec3(u_model * vec4(v_in_position, 1.0));
    v_out_normal = mat3(transpose(inverse(u_model))) * v_in_normal;
    gl_Position = u_viewProjection * vec4(v_out_position, 1.0);
}

