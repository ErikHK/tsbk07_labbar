#version 130

in  vec3 in_Position;
in vec2 inTexCoord;
out vec2 texCoordV;
out vec3 out_Position;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
	gl_Position = projMatrix*viewMatrix*mdlMatrix*vec4(in_Position, 1.0);
	texCoordV = inTexCoord;
	out_Position = in_Position;
}
