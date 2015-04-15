#version 150

//in vec3 position;
//in vec4 color;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

in vec3 myColor;
out vec3 norm;
out vec3 vec;

out vec3 Normal;

out vec4 color;
out vec2 texCoordV;
out vec3 out_Position;
uniform mat4 myMatrix;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texUnit;

mat3 normalMatrix = mat3(mdlMatrix);
vec3 transformedNormal = normalMatrix * in_Normal;

void main(void)
{
	Normal = transformedNormal;
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(in_Normal), light);
	color = vec4(shade);
	
	//color = vec4(in_Position+1.0, 1.0);

	gl_Position = projMatrix*camMatrix*mdlMatrix*(vec4(in_Position, 1.0));
	texCoordV = inTexCoord;
}