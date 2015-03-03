#version 130

//varying vec4 position;
in vec2 texCoordV;
in vec4 color;
in vec3 in_Position;
out vec4 out_Color;
uniform mat4 myMatrix;
uniform sampler2D texUnit;

in vec3 norm;
in vec3 vec;

void main(void)
{
	vec3 L = normalize(gl_LightSource[0].position.xyz-vec);
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(norm, L),0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);

	//out_Color = vec4(1.0);
	//out_Color = vec4(0.5, 0.5, 1.0, 1.0);
	//out_Color = color;
	//gl_FragColor = color;
	//out_Color = myMatrix*vec4(1.0,1.0,1.0,1.0);
	//out_Color = color;
	//out_Color = myMatrix*vec4(texCoordV,0.0,0.0)*sin((myMatrix[0]*10)+1.5); //animated!
	//out_Color = vec4(texCoordV,0.0, 0.0);
	//out_Color = texture(texUnit, texCoordV*sin(myMatrix[0][0])); //animated texture!
	//out_Color = texture(texUnit, texCoordV);
	//gl_FragColor = Idiff;
	out_Color = vec4(color)*texture(texUnit, texCoordV);
}
