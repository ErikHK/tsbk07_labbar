#version 130

in vec4 color;
out vec4 out_Color;
uniform mat4 myMatrix;

void main(void)
{
	out_Color = color;
}
