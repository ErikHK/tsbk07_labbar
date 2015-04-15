// Lab 1-6.
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
#include <math.h>

// Globals
// Data would normally be read from files

GLfloat vertices[] =
{
	-0.5f,-0.5f,-0.5f, //1
	-0.5f,0.5f,-0.5f,
	0.5f,0.5f,-0.5f,
	-0.5f, -0.5f, -0.5f, //2
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, //3
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, //4
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f, //5
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, //6
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f, //7
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f, //8
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f, //9
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, -0.5f, //10
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f, //11
	-0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f, //12
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f


};

GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};

GLfloat myColor[] = {1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f,  1.0f};


unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
Model *m;

// Reference to shader program
GLuint program;

// vertex array object
unsigned int vertexArrayObjID;


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
}


void init(void)
{
	m = LoadModel("bunny.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vbo_colors;

	//init timer
	glutTimerFunc(20, &OnTimer, 0);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glBindVertexArray(bunnyVertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &vbo_colors);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(myColor), myColor, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "myColor"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "myColor"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
	
	// End of upload of geometry
	
	printError("init arrays");

	//upload matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	glBindVertexArray(bunnyVertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Labb 1-6");
	glutDisplayFunc(display); 
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
