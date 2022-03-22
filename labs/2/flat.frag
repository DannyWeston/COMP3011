#version 330 core

out vec4 fragColour;

in vec3 col;

void main()
{
	float red = mod(int(col.r * 2), 2);
	float blue = mod(int(col.b * 2), 2);

	fragColour = vec4(red, col.g, blue, 1.0f);
}