#version 330 core

out vec4 fragColour;

in vec3 col;

void main()
{
	fragColour = vec4(col.r, col.g, col.b, 1.0f);
}