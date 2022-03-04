#version 330 core

out vec4 fragColour;

in vec3 col;

void main()
{
	fragColour = vec4(col, 1.0f);
}