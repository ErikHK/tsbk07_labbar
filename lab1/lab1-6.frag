#version 130

//varying vec4 position;
in vec4 color;
out vec4 out_Color;
uniform mat4 myMatrix;

void main(void)
{
	//out_Color = vec4(1.0);
	//out_Color = vec4(0.5, 0.5, 1.0, 1.0);
	//out_Color = color;
	//gl_FragColor = color;
	//out_Color = myMatrix*vec4(1.0,1.0,1.0,1.0);
	out_Color = color;
}
