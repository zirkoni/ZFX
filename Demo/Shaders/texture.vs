#version 330

layout (location = 0) in vec2 positionIn;
layout (location = 1) in vec2 texCoordIn;

uniform mat4 model;
uniform mat4 viewProjection;
varying vec2 texCoord;

void main()
{
    gl_Position = viewProjection * model * vec4(positionIn, 0.0, 1.0);
    texCoord = texCoordIn;
}

