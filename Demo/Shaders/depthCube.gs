#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 g_out_fragPos;

uniform mat4 shadowMatrices[6];

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.

        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            g_out_fragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * g_out_fragPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}
