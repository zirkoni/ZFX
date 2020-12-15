#version 330
in vec2 position;

out vec4 fragColorOut;
uniform vec4 colour;
uniform float radius = 0.5;

void main()
{
    // Set only pixels inside the circle
    float distance = length(vec3(position, 0.0));
	float inside = 1.0 - step(radius, distance);
	fragColorOut = colour * inside;
}