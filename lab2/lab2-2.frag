#version 130

in vec2 texCoordV;
in vec4 color;
in vec3 in_Position;
out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, texCoordV);
	//out_Color = texture(texUnit, texCoordV*sin(myMatrix[0][0]));
}
