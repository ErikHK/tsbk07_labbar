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
	//out_Color = vec4(1.0);
	//out_Color = vec4(0.5, 0.5, 1.0, 1.0);
	//out_Color = color;
	//gl_FragColor = color;
	//out_Color = myMatrix*vec4(1.0,1.0,1.0,1.0);
	//out_Color = color;
	//out_Color = myMatrix*vec4(texCoordV,0.0,0.0)*sin((myMatrix[0]*10)+1.5); //animated!
	//out_Color = vec4(texCoordV,0.0, 0.0);
	out_Color = texture(texUnit, texCoordV*sin(myMatrix[0][0]));
}
