#version 330

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordIn;

out vec3 position;
out vec3 normal;
varying vec2 texCoord;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    position = vec3(model * vec4(positionIn, 1.0));
    normal = mat3(transpose(inverse(model))) * normalIn;
    texCoord = texCoordIn;
    gl_Position = viewProjection * vec4(position, 1.0);
}

