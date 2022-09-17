#version 330 core

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordIn;

out VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} vs_out;

uniform mat4 viewProjection;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.fragPos = vec3(model * vec4(positionIn, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * normalIn;
    vs_out.texCoords = texCoordIn;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos, 1.0);
    gl_Position = viewProjection * model * vec4(positionIn, 1.0);
}
