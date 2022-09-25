#version 330 core
layout (location = 0) in vec3 positionIn;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(positionIn, 1.0);
}
