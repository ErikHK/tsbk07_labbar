#version 130

//varying vec4 position;
in vec2 texCoordV;
in vec4 color;
in vec3 in_Position;
out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;

void main(void)
{
	//out_Color = texture(texUnit, texCoordV*sin(myMatrix[0][0])); //animated texture!
	out_Color = texture(texUnit, texCoordV);
}
