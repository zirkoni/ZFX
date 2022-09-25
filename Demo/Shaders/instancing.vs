#version 330

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in mat4 v_in_modelMatrix;

out vec3 v_out_position;
out vec3 v_out_normal;

uniform mat4 u_viewProjection;

void main()
{
    v_out_position = vec3(v_in_modelMatrix * vec4(v_in_position, 1.0));
    v_out_normal = mat3(transpose(inverse(v_in_modelMatrix))) * v_in_normal; // TODO: inverse is slow => calculate v_out_normal matrix on CPU
    gl_Position = u_viewProjection * vec4(v_out_position, 1.0);
}

