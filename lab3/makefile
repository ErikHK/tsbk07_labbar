
all : lab3-1

lab3-1: lab3-1.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab3-1.c loadobj.c LoadTGA.c VectorUtils3.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab3-1



clean :
	rm -f lab3-1
