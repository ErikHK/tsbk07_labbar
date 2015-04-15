#version 150

//varying vec4 position;
in vec2 texCoordV;
in vec4 color;
//in vec3 in_Position;
//in vec3 in_Position2;
out vec4 out_Color;

uniform mat4 myMatrix;
uniform sampler2D texUnit;
uniform int skybox;
uniform int specular;
uniform mat4 camMatrix;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

mat3 lightCamMatrix = mat3(camMatrix);

in vec3 norm;
in vec3 vec;

void main(void)
{
	if(skybox){
		//out_Color = vec4(color)*texture(texUnit, texCoordV);
		out_Color = texture(texUnit, texCoordV);
	} else {
	
	if(specular)
	{
		//colors = vec4(1,0,0,1);
		//out_Color = colors;
	}
	
	else //just diffuse shading plix
	{
		out_Color = vec4(color)*texture(texUnit, texCoordV);	
	}
	
	}
	
}
