// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include <math.h>



#define near 1.0
#define far 400.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };


// Globals
// Data would normally be read from files


GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};


Point3D lightSourcesColorsArr[] = { { 1.0f, 0.0f, 1.0f },
{ 1.0f, 1.0f, 1.0f },
{ 1.0f, 1.0f, 1.0f },
{ 1.0f, 1.0f, 1.0f } }; // White light

GLfloat specularExponent[] = { 1.0, 20.0, 60.0, 5.0 };
GLint isDirectional[] = { 1, 1, 1, 1 };

Point3D lightSourcesDirectionsPositions[] = { { 10.0f, 5.0f, 0.0f }, // Red light, positional
{ 0.0f, 5.0f, 10.0f }, // Green light, positional
{ -1.0f, 0.0f, 0.0f }, // Blue light along X
{ 0.0f, 0.0f, -1.0f } }; // White light along Z


Model *balcony, *roof, *walls;
Model * blades[4];
Model *ground;
Model *cow;
Model *skybox;
// Reference to shader program
GLuint program;

unsigned int skyboxTex;
unsigned int groundTex;

// vertex array object
unsigned int vertexArrayObjID;
mat4 rot, mill, look, total, balcony_trans, walls_trans, roof_trans, ground_trans, cow_trans;
mat4 blades_rot[4];
mat4 blades_anim[4];
mat4 * curr_trans;

void OnTimer(int value)
{
    static float x = 20;
    static float y = 13;
    static float z = 20;
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
    if (keyIsDown('w'))
    	x += 0.5;
    if (keyIsDown('s'))
    	x -= 0.5;
    if (keyIsDown('a'))
    	z -= 0.5;
    if(keyIsDown('d'))
    	z += 0.5; 
    look = lookAt(x, 13 ,z,  0,0,0,  0,1,0);
    myMatrix[0] = cos(t)*cos(t);
    myMatrix[1] = -sin(t);
    myMatrix[2] = sin(t)*cos(t);
    myMatrix[4] = sin(t)*sin(t)+cos(t)*cos(t)*sin(t);
    myMatrix[5] = cos(t)*cos(t);
    myMatrix[6] = cos(t)*sin(t)*sin(t)-sin(t)*cos(t);
    myMatrix[8] = cos(t)*sin(t)*sin(t)-sin(t)*cos(t);
    myMatrix[9] = cos(t)*sin(t);
    myMatrix[10] = cos(t)*cos(t)+sin(t)*sin(t)*sin(t);

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);


    static float a = 0;
    a += 0.03;
    rot = Rx(a);
	for(int i=0;i<4;i++)
    {
        blades_anim[i] = Mult(T(4.7,10,3), Mult(rot, blades_rot[i]));
    }

}


void init(void)
{
	mill = T(0,0,3);
	balcony_trans = T(0,1,3);
	walls_trans = T(0,1,3);
	roof_trans = T(0,1,3);
	ground_trans = T(0,0,0);
	cow_trans = Mult(T(10,2,10), S(3, 3, 3));

	blades_rot[0] = T(0,1,0);
	blades_rot[1] = Mult(Rx(M_PI/2), T(0,1,0));
	blades_rot[2] = Mult(Rx(M_PI), T(0,1,0));
	blades_rot[3] = Mult(Rx(3*M_PI/2), T(0,1,0));
	
	look = lookAt(20,13,20,  0,0,0,  0,1,0);

	balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	walls = LoadModelPlus("windmill/windmill-walls.obj");
	roof = LoadModelPlus("windmill/windmill-roof.obj");
	ground = LoadModelPlus("ground.obj");
	cow = LoadModelPlus("cow.obj");
	skybox = LoadModelPlus("skybox.obj");
	
	blades[0] = LoadModelPlus("windmill/blade.obj");
	blades[1] = LoadModelPlus("windmill/blade.obj");
	blades[2] = LoadModelPlus("windmill/blade.obj");
	blades[3] = LoadModelPlus("windmill/blade.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vbo_colors;
	

	//load texture
	LoadTGATextureSimple("grass.tga", &groundTex);
	LoadTGATextureSimple("SkyBox512.tga", &skyboxTex);

	initKeymapManager();
	//init timer
	glutTimerFunc(20, &OnTimer, 0);

	dumpInfo();

	// GL inits
	glClearColor(1,1,1,0);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-5.vert", "lab3-5.frag");
	printError("init shader");

	//upload matrices
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mill.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);

	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "texUnit2"), 1); // Texture unit 1
}


void display(void)
{
	printError("pre display");
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(program, "skybox"), 1);


	GLfloat skyboxmat[16];
	memcpy(skyboxmat, look.m, sizeof(skyboxmat));
	skyboxmat[3] = 0;
	skyboxmat[7] = -0.05;
	skyboxmat[11] = 0;
	skyboxmat[15] = 1;

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, skyboxmat);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, IdentityMatrix().m);
	
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	DrawModel(skybox, program, "in_Position", "in_Normal", "inTexCoord");


	glEnable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);

	//draw multi-tex
	glUniform1i(glGetUniformLocation(program, "multitex"), 1);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, cow_trans.m);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, groundTex);
	DrawModel(cow, program, "in_Position", "in_Normal", "inTexCoord");


    glUniform1i(glGetUniformLocation(program, "multitex"), 0);


	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, balcony_trans.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);
	DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, walls_trans.m);
	DrawModel(walls, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, roof_trans.m);
	DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, ground_trans.m);
	DrawModel(ground, program, "in_Position", "in_Normal", "inTexCoord");

	//glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, cow_trans.m);
	//DrawModel(cow, program, "in_Position", "in_Normal", "inTexCoord");
	

	for(int i=0;i<4;i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, blades_anim[i].m);

		DrawModel(blades[i], program, "in_Position", "in_Normal", "inTexCoord");
	}


	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(800, 600);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
