#version 130

in vec2 texCoordV;
in vec4 color;

in vec3 ex_Normal;

out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;
uniform int skybox;

in vec3 norm;
in vec3 vec;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;

	shade = dot(normalize(ex_Normal), light);


	if(skybox==1)
	{
		out_Color = texture(texUnit, texCoordV);

	}
	else{
		out_Color = vec4(shade, shade, shade, 1.0)*texture(texUnit, texCoordV);

	}


	//out_Color = vec4(color)*texture(texUnit, texCoordV);
	//out_Color = texture(texUnit, texCoordV);
	//out_Color = vec4(color);
}
