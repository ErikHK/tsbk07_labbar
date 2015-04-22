#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
in vec3 myColor;

out vec3 norm;
out vec3 vec;
out vec3 ex_Normal;
out vec3 NormalTex;

out vec4 color;
out vec2 texCoordV;

out vec3 outPosition;
out vec3 outPositionCam;

uniform mat4 myMatrix;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texUnit;


mat3 normalMatrix = mat3(camMatrix * mdlMatrix);
vec3 transformedNormal = normalMatrix * in_Normal;

void main(void)
{

	//const vec3 light = vec3(0.58, 0.58, 0.58);
	//const vec3 light = vec3(0, 0, 0.58);
	//float shade;
	//shade = dot(normalize(in_Normal), normalize(light));
	//color = vec4(shade)+ vec4(1,1,1,1);

	NormalTex = in_Normal;

	ex_Normal = transformedNormal;
	gl_Position = projMatrix*camMatrix*mdlMatrix*(vec4(in_Position, 1.0));
	texCoordV = inTexCoord;

	outPositionCam = vec3(camMatrix * mdlMatrix * vec4(in_Position, 1));
	outPosition = mat3(camMatrix) * vec3(mdlMatrix * vec4(in_Position, 1));
}
