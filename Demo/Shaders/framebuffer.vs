#version 330

layout (location = 0) in vec2 positionIn;
layout (location = 1) in vec2 texCoordIn;

out vec2 texCoord;

void main()
{
    texCoord = texCoordIn;
    gl_Position = vec4(positionIn, 0.0, 1.0);
}

