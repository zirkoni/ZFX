#version 330
#define ANTIALIASED

in vec2 position;

out vec4 fragColorOut;
uniform vec4 colour;

void main()
{
    float radius = 0.5;

#ifndef ANTIALIASED
    float distance = length(vec3(position, 0.0));
	float inside = 1.0 - step(radius, distance);
	fragColorOut = colour * inside;
#else
    float distance = distance(position, vec2(0.0, 0.0));
    float delta = fwidth(distance);
    float alpha = smoothstep(radius - delta, radius, distance);
    vec4 bgColour = vec4(0.0);
    fragColorOut = mix(colour, bgColour, alpha);
#endif

}
