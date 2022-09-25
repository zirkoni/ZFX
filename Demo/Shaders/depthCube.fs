#version 330 core
in vec4 g_out_fragPos;

uniform vec3 u_lightPos;
uniform float u_farPlane;

void main()
{
    float lightDistance = length(g_out_fragPos.xyz - u_lightPos);

    // map to [0;1] range by dividing by farPlane
    lightDistance = lightDistance / u_farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
