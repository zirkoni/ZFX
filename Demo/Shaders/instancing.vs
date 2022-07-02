#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in mat4 modelMatrixIn;

out vec3 position;
out vec3 normal;

uniform mat4 viewProjection;

void main()
{
    position = vec3(modelMatrixIn * vec4(positionIn, 1.0));
    normal = mat3(transpose(inverse(modelMatrixIn))) * normalIn; // TODO: inverse is slow => calculate normal matrix on CPU
    gl_Position = viewProjection * vec4(position, 1.0);
}

