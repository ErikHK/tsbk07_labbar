#version 130

//in vec2 texCoordV;
in vec4 color;
out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;

in vec3 norm;
in vec3 vec;

void main(void)
{
	//out_Color = vec4(color)*texture(texUnit, texCoordV);
	out_Color = vec4(color);
}
