#version 130

//in vec3 position;
//in vec4 color;

in  vec3 in_Position;
in vec2 inTexCoord;
in vec3 myColor;
out vec4 color;
out vec2 texCoordV;
out vec3 out_Position;
uniform mat4 myMatrix;

void main(void)
{
	gl_Position = myMatrix*vec4(in_Position[0], in_Position[1], in_Position[2]/1.5, 1.0);
	//color = vec4(in_Position, 1.0);
	//color = myColor*vec4(in_Position, 1.0);
	//color = vec4(1.0, 1.0, 0.0, 1.0);
	//color = vec4(myColor, 1.0);
	color = vec4(in_Position+.5, 1.0);
	//color = inTexCoord;
	//color = vec4(inTexCoord, 1.0, 1.0);
	texCoordV = inTexCoord;
	out_Position = in_Position;
}
