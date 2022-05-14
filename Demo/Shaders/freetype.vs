#version 330 core
layout (location = 0) in vec4 vertex; // xy = vec2 position, zw= vec2 texture coordinates
out vec2 texCoord;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(vertex.xy, 0.0, 1.0);
    texCoord = vertex.zw;
}
