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
#define far 40.0
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

Model *balcony, *roof, *walls;
Model * blades[4];
// Reference to shader program
GLuint program;

// vertex array object
unsigned int vertexArrayObjID;
mat4 rot, rot2, trans1, trans2, trans1_2, trans2_2, trans3, trans3_2, trans4, trans4_2, trans0, trans0_2, look, total;
mat4 blades_rot[4];
mat4 blades_anim[4];
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
        blades_anim[i] = Mult(T(4.7,9,3), Mult(rot2, blades_rot[i]));
    }

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

	blades_rot[0] = T(0,0,0);
	blades_rot[1] = Mult(Rx(M_PI/2), T(0,0,0));
	blades_rot[2] = Mult(Rx(M_PI), T(0,0,0));
	blades_rot[3] = Mult(Rx(3*M_PI/2), T(0,0,0));
	//rot = Mult(Ry(M_PI/4), Rx(M_PI/8));

	//rot = Ry(M_PI/4);

	//total = Mult(trans, rot);

	look = lookAt(20,13,20,  0,0,0,  0,1,0);

	balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	walls = LoadModelPlus("windmill/windmill-walls.obj");
	roof = LoadModelPlus("windmill/windmill-roof.obj");
	
	blades[0] = LoadModelPlus("windmill/blade.obj");
	blades[1] = LoadModelPlus("windmill/blade.obj");
	blades[2] = LoadModelPlus("windmill/blade.obj");
	blades[3] = LoadModelPlus("windmill/blade.obj");

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
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
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

	

	DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");
	//curr_trans = &trans2;

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans2_2.m);

	DrawModel(walls, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans3_2.m);

	DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");

	

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
