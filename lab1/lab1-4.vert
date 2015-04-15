#version 130

in  vec3 in_Position;
in vec3 myColor;
out vec4 color;

uniform mat4 myMatrix;

void main(void)
{
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	color = vec4(myColor, 1.0);
}
