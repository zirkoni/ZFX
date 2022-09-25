#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordIn;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;

uniform mat4 viewProjection;
uniform mat4 model;

uniform bool reverseNormals;

void main()
{
    vs_out.fragPos = vec3(model * vec4(positionIn, 1.0));

    // A slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
    if(reverseNormals)
    {
        vs_out.normal = transpose(inverse(mat3(model))) * (-1.0 * normalIn);
    } else
    {
        vs_out.normal = transpose(inverse(mat3(model))) * normalIn;
    }

    vs_out.texCoords = texCoordIn;
    gl_Position = viewProjection * model * vec4(positionIn, 1.0);
}
