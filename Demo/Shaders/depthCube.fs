#version 330 core
in vec4 g_out_fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float lightDistance = length(g_out_fragPos.xyz - lightPos);

    // map to [0;1] range by dividing by farPlane
    lightDistance = lightDistance / farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
