#version 130

in vec2 texCoordV;
in vec4 color;
out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;
uniform int skybox;

in vec3 norm;
in vec3 vec;

void main(void)
{
	if(skybox==1)
	{
		out_Color = texture(texUnit, texCoordV);

	}
	else{
		out_Color = vec4(color)*texture(texUnit, texCoordV);

	}


	//out_Color = vec4(color)*texture(texUnit, texCoordV);
	//out_Color = texture(texUnit, texCoordV);
	//out_Color = vec4(color);
}
