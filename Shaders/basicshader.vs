#version 330

attribute vec3 positionIn;

varying vec3 positionOut;

void main()
{
	positionOut = positionIn;
	gl_Position = vec4(positionIn, 1.0);
}
