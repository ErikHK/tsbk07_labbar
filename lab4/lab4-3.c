// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;


Point3D lightSourcesColorsArr[] = { { 1.0f, 0.0f, 1.0f },
{ 0.0f, 0.0f, 1.0f },
{ 1.0f, 0.0f, 0.0f },
{ 1.0f, 1.0f, 1.0f } };

GLfloat specularExponent[] = { 1.0, 20.0, 60.0, 5.0 };
GLint isDirectional[] = { 0, 0, 1, 1 };

Point3D lightSourcesDirectionsPositions[] = { { 0.0f, 5.0f, 0.0f }, // Red light, positional
{ 0.0f, 50.0f, 10.0f }, // Green light, positional
{ -1.0f, 2.0f, 0.0f }, // Blue light along X
{ 0.0f, 1.0f, -1.0f } }; // White light along Z



//theta left right 360, phi up down 180
float phi=0, theta=0;
mat4 total, modelView, camMatrix;

void calc_normal(GLfloat *vertexArray, int x, int z, int width, Point3D *normal)
{
	Point3D vec1, vec2;

	if(x > 0 && z > 0 && x < width && z < width)
	{
		vec1.x = vertexArray[(x-1 + z * width)*3 + 0] - 
		vertexArray[(x + z * width)*3 + 0];

		vec1.y = vertexArray[(x-1 + z * width)*3 + 1] - 
		vertexArray[(x + z * width)*3 + 1];

		vec1.z = vertexArray[(x-1 + z * width)*3 + 2] - 
		vertexArray[(x + z * width)*3 + 2];


		vec2.x = vertexArray[(x + (z+1) * width)*3 + 0] - 
		vertexArray[(x + z * width)*3 + 0];

		vec2.y = vertexArray[(x + (z+1) * width)*3 + 1] - 
		vertexArray[(x + z * width)*3 + 1];

		vec2.z = vertexArray[(x + (z+1) * width)*3 + 2] - 
		vertexArray[(x + z * width)*3 + 2];

		
		*normal = Normalize(CrossProduct(vec1, vec2));

	}
}



Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	Point3D tmp_normal;
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
		// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 4.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 4.0;
		// Normal vectors. You need to calculate these.

			calc_normal(vertexArray, x, z, tex->width, &tmp_normal);
			printf("%f %f %f\n", tmp_normal.x, tmp_normal.y, tmp_normal.z);


			normalArray[(x + z * tex->width)*3 + 0] = tmp_normal.x;
			normalArray[(x + z * tex->width)*3 + 1] = tmp_normal.y;
			normalArray[(x + z * tex->width)*3 + 2] = tmp_normal.z;
		// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
vec3 lookAtPoint = {4, 0, 4};
vec3 lookAtPoint_tmp = {4, 0, 4};
vec3 cam = {0, 5, 8};


void mouse(int x, int y)
{
	float phi_m = ((float)x)/600*2*M_PI;
	float theta_m = ((float)y)/600*M_PI;

	lookAtPoint.x = -10*sin(theta_m)*sin(phi_m) + cam.x;
	lookAtPoint.y = 10*cos(theta_m) + cam.y;
	lookAtPoint.z = 10*sin(theta_m)*cos(phi_m) + cam.z;
}


void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 75.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	
	//mat4 roty = Ry(theta);
	//lookAtPoint = MultVec3(roty, lookAtPoint_tmp);

	camMatrix =  lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{


	vec3 test = VectorSub(cam, lookAtPoint);
	float looknorm = sqrt(pow(test.x,2) + 
		pow(test.y,2) + pow(test.z,2));
	float speed = 0.1;
	

	if (keyIsDown('w'))
	{
		cam.x -= speed*test.x/(looknorm);
		cam.y -= speed*test.y/(looknorm);
		cam.z -= speed*test.z/(looknorm);
		
	}

	if (keyIsDown('s'))
	{
		cam.x += speed*test.x/(looknorm);
		cam.y += speed*test.y/(looknorm);
		cam.z += speed*test.z/(looknorm);	
	}

	//strafe left
	if (keyIsDown('a'))
	{
		cam.x -= speed*test.z/(looknorm);
		cam.y -= speed*test.y/(looknorm);
		cam.z += speed*test.x/(looknorm);
		
	}

	//strafe right
	if (keyIsDown('d'))
	{
		cam.x += speed*test.z/(looknorm);
		cam.y -= speed*test.y/(looknorm);
		cam.z -= speed*test.x/(looknorm);
		
	}



		
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
