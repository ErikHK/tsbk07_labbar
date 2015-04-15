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


Model *m, *m2, *golv;

// Reference to shader program
GLuint program;

// vertex array object
unsigned int vertexArrayObjID;
mat4 rot, trans1, trans2, trans1_2, trans2_2, trans0, trans0_2, look, total;
mat4 * curr_trans;

void OnTimer(int value)
{
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


    static float a = 0;
    a += 0.03;
    rot = Ry(a);
    //curr_trans_tot = &trans1;
    //*curr_trans = Mult(rot, *curr_trans);
    //total = Mult(rot, look);
    trans1_2 = Mult(rot, trans1);
    trans2_2 = Mult(rot, trans2);
    trans0_2 = rot;

}


void init(void)
{



	trans1 = T(0,0,3);
	trans1_2 = T(0,0,3);
	trans2 = T(0,0,-3);
	trans2_2 = T(0,0,-3);
	trans0 = T(0,0,0);

	//rot = Mult(Ry(M_PI/4), Rx(M_PI/8));

	//rot = Ry(M_PI/4);

	//total = Mult(trans, rot);

	look = lookAt(0,3,5,  0,0,0,  0,1,0);

	m = LoadModelPlus("bunnyplus.obj");
	m2 = LoadModelPlus("models/various/cow.obj");
	golv = LoadModelPlus("golv.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vbo_colors;
	unsigned int myTex;

	//load texture
	LoadTGATextureSimple("maskros512.tga", &myTex);

	//init timer
	glutTimerFunc(20, &OnTimer, 0);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");

	//upload matrices
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans1_2.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);

	//curr_trans = &trans1;
	//rot = Ry(M_PI/4);
	//*curr_trans = Mult(rot, trans1);
	//*curr_trans_tot = Mult(rot, *curr_trans);


}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//curr_trans = &trans1;

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans1_2.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, look.m);

	DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
	//curr_trans = &trans2;

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans2_2.m);

	DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");

	//glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans0_2.m);

	//DrawModel(golv, program, "in_Position", "in_Normal", "inTexCoord");


	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
