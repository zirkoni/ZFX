#version 330
#define ANTIALIASED

in vec2 v_out_position;

out vec4 f_out_colour;
uniform vec4 u_colour;

void main()
{
    float radius = 0.5;

#ifndef ANTIALIASED
    float distance = length(vec3(v_out_position, 0.0));
    float inside = 1.0 - step(radius, distance);
    f_out_colour = u_colour * inside;
#else
    float distance = distance(v_out_position, vec2(0.0, 0.0));
    float delta = fwidth(distance);
    float alpha = smoothstep(radius - delta, radius, distance);
    vec4 bgColour = vec4(0.0);
    f_out_colour = mix(u_colour, bgColour, alpha);
#endif

}
