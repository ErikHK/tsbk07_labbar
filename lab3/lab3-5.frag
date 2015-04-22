#version 130

in vec2 texCoordV;
in vec4 color;

in vec3 ex_Normal;

in vec3 outPosition;
in vec3 outPositionCam;

uniform mat4 camMatrix;

out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit, texUnit2;
uniform int skybox;

uniform bool multitex;

in vec3 norm;
in vec3 vec;
vec3 colors;

vec3 s;
vec3 n;
vec3 eye;

float cos_angle;

vec3 diffuse;
vec3 specular;
vec3 tmp_colors;
vec3 reflection;


uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

mat3 lightCamMatrix = mat3(camMatrix);

void main(void)
{
	colors = vec3(0,0,0);
	colors += vec3(0.5, 0.5, 0.5);


	if(multitex)
	{
    	tmp_colors = vec3(texture(texUnit, texCoordV));
    }
    else{
    	tmp_colors = vec3(texture(texUnit2, texCoordV));
    }


	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;

	shade = dot(normalize(ex_Normal), light);


	if(skybox==1)
	{
		out_Color = texture(texUnit, texCoordV);

	}
	else{

        for(int i=0;i<4;i++){
        	if(isDirectional[i])
        	{
        		s = normalize(lightCamMatrix * lightSourcesDirPosArr[i]);
        	}else{
                s = normalize(lightCamMatrix * lightSourcesDirPosArr[i]-outPosition);
        	}
        	n = normalize(ex_Normal);

        	float lambert = dot(n,s)-.001;

        	if(lambert > 0)
        	{
				diffuse = (lightSourcesColorArr[i]*tmp_colors)*lambert;
				colors += diffuse;

				reflection = reflect(s, n);
				eye = normalize(outPositionCam);

				cos_angle = dot(reflection, eye);
				cos_angle = max(0, cos_angle);

				specular = (lightSourcesColorArr[i]*tmp_colors)*pow(cos_angle, specularExponent[i]);
				colors += specular;

        	}
		}


		//out_Color = vec4(shade, shade, shade, 1.0)*texture(texUnit, texCoordV);
		out_Color = vec4(colors, 1.0)*texture(texUnit, texCoordV);

	}


	//out_Color = vec4(color)*texture(texUnit, texCoordV);
	//out_Color = texture(texUnit, texCoordV);
	//out_Color = vec4(color);
}
