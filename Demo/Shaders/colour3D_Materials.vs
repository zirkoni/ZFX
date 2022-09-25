#version 330

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;

out vec3 v_out_position;
out vec3 v_out_normal;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    v_out_position = vec3(model * vec4(v_in_position, 1.0));
    v_out_normal = mat3(transpose(inverse(model))) * v_in_normal;
    gl_Position = viewProjection * vec4(v_out_position, 1.0);
}

