// Lab 3-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#include "windows.h"

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
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };


GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};


Point3D lightSourcesColorsArr[] = { { 1.0f, 0.0f, 0.0f }, // Red light
{ 0.0f, 1.0f, 0.0f }, // Green light
{ 0.0f, 0.0f, 1.0f }, // Blue light
{ 1.0f, 1.0f, 1.0f } }; // White light

GLfloat specularExponent[] = { 10.0, 20.0, 60.0, 5.0 };
GLint isDirectional[] = { 0, 0, 1, 1 };

Point3D lightSourcesDirectionsPositions[] = { { 10.0f, 5.0f, 0.0f }, // Red light, positional
{ 0.0f, 5.0f, 10.0f }, // Green light, positional
{ -1.0f, 0.0f, 0.0f }, // Blue light along X
{ 0.0f, 0.0f, -1.0f } }; // White light along Z




Model *m, *m2, *m3, *m4, *skybox, *bunny, *cube, *sphere, *ground;
Model * blades[4];
// Reference to shader program
GLuint program;
int specular = 0;

// vertex array object
unsigned int vertexArrayObjID;
mat4 rot, rot2, trans1, trans2, trans1_2, trans2_2, trans3, trans3_2, trans4, trans4_2, trans0, trans0_2, look, skyboxmat, bunnymat, cubemat, spheremat, groundmat, total;
mat4 trans_blades[4];
mat4 trans_blades_2[4];
mat4 * curr_trans;
unsigned int myTex, groundTex;

double lookx=0, looky=0, lookz=0, zoom=15;

void OnTimer(int value)
{
	glUniform1i(glGetUniformLocation(program, "specular"), specular);

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
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


    static float a = 0, a2 = 0;
    //a += 0.03;
    a2 += 0.03;
    rot = Ry(a);
    rot2 = Rx(a2);
    //curr_trans_tot = &trans1;
    // *curr_trans = Mult(rot, *curr_trans);
    //total = Mult(rot, look);
    trans1_2 = Mult(rot, trans1);
    trans2_2 = Mult(rot, trans2);
    trans3_2 = Mult(rot, trans3);
    trans4_2 = Mult(rot, trans4);
    trans0_2 = rot;


    for(int i=0;i<4;i++)
    {
        trans_blades_2[i] = Mult(T(4.7,9,3), Mult(rot2, trans_blades[i]));
    }

	static double angle = 0;
	if (keyIsDown('a'))
		angle+=.05;
	if (keyIsDown('b'))
		angle-=.05;

	if (keyIsDown('+'))
		zoom -= .5;
	if (keyIsDown('-'))
		zoom += .5;


	if (keyIsDown('s'))
		specular = 1;

	if (keyIsDown('d'))
		specular = 0;

	lookx = zoom * sin(angle);
	lookz = zoom * cos(angle);
	look = lookAt(lookx,looky,lookz,  0.1,7,0.1,  0,1,0);
	//skyboxmat = look;
	//CopyMatrix(look.m, skyboxmat.m);
	//skyboxmat.m[3] = 0;
	//skyboxmat.m[7] = 0;
	//skyboxmat.m[11] = 0;
	//skyboxmat.m[15] = 1;

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);
}


void init(void)
{



	trans1 = T(0,0,3);
	trans1_2 = T(0,0,3);
	trans2 = T(0,0,3);
	trans2_2 = T(0,0,3);
	trans3 = T(0,0,3);
	trans4 = T(5,6,3);
	trans0 = T(0,0,0);
	bunnymat = T(10, 0, 0);
	cubemat = T(0, 0, 10);
	spheremat = T(20, 0, 0);
	groundmat = T(0, 0, 0);

	trans_blades[0] = T(0,0,0);
	trans_blades[1] = Mult(Rx(M_PI/2), T(0,0,0));
	trans_blades[2] = Mult(Rx(M_PI), T(0,0,0));
	trans_blades[3] = Mult(Rx(3*M_PI/2), T(0,0,0));
	//rot = Mult(Ry(M_PI/4), Rx(M_PI/8));

	//rot = Ry(M_PI/4);

	//total = Mult(trans, rot);
	
	lookx = 20;
	looky = 13;
	lookz = 20;
	
	look = lookAt(lookx,looky,lookz,  0.1,0.1,0.1,  0,1,0);

	m = LoadModelPlus("windmill/windmill-balcony.obj");
	m2 = LoadModelPlus("windmill/windmill-walls.obj");
	m3 = LoadModelPlus("windmill/windmill-roof.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	skybox = LoadModelPlus("skybox.obj");
	bunny = LoadModelPlus("bunnyplus.obj");
	cube = LoadModelPlus("models/various/cube.obj");
	sphere = LoadModelPlus("sphere.obj");
	ground = LoadModelPlus("ground.obj");

	blades[0] = LoadModelPlus("windmill/blade.obj");
	blades[1] = LoadModelPlus("windmill/blade.obj");
	blades[2] = LoadModelPlus("windmill/blade.obj");
	blades[3] = LoadModelPlus("windmill/blade.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vbo_colors;
	

	//load texture
	LoadTGATextureSimple("maskros512.tga", &myTex);
	LoadTGATextureSimple("golv.tga", &groundTex);
	glBindTexture(GL_TEXTURE_2D, myTex);

	//init timer
	glutTimerFunc(20, &OnTimer, 0);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

	//upload matrices
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans1_2.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);


	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	//glActiveTexture(GL_TEXTURE0);

	//curr_trans = &trans1;
	//rot = Ry(M_PI/4);
	//*curr_trans = Mult(rot, trans1);
	//*curr_trans_tot = Mult(rot, *curr_trans);
}

void display(void)
{
	printError("pre display");

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(program, "skybox"), 1);

	//curr_trans = &trans1;

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);

	GLfloat skyboxmat[16];
	memcpy(skyboxmat, look.m, sizeof(skyboxmat));
	skyboxmat[3] = 0;
	skyboxmat[7] = 0;
	skyboxmat[11] = 0;
	skyboxmat[15] = 1;

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, skyboxmat);

	//LoadTGATextureSimple("maskros512.tga", &myTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, IdentityMatrix().m);
	LoadTGATextureSimple("SkyBox512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(skybox, program, "in_Position", "in_Normal", "inTexCoord");


	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);


	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans1_2.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
	//curr_trans = &trans2;

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans2_2.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans3_2.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(m3, program, "in_Position", "in_Normal", "inTexCoord");


	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bunnymat.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(bunny, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, cubemat.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(cube, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, spheremat.m);
	glBindTexture(GL_TEXTURE_2D, myTex);
	DrawModel(sphere, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, groundmat.m);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	DrawModel(ground, program, "in_Position", "in_Normal", "inTexCoord");

	
	for(int i=0;i<4;i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans_blades_2[i].m);
		DrawModel(blades[i], program, "in_Position", "in_Normal", "inTexCoord");
	}
	

	// clear the screen
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	//glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

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
	glewInit();
	init ();
	initKeymapManager();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
